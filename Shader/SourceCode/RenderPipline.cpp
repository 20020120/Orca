#include"RenderPipline.h"
#include"ShaderBuilder.h"
#include"MacroMinmam.h"
#include"OrcaException.h"
#include "GraphicsLogger.h"

#include"InputLayout.h"
#include"BlendStates.h"
#include"SamplerStates.h"
#include"RasterizerStates.h"
#include"DepthStencilStates.h"

#include"ReflectionHelpers.h"

#include<filesystem>

OrcaGraphics::RenderPipeline::RenderPipeline(OrcaComPtr(ID3D12Device) pDevice_, const Shader::ShaderDesc& ShaderDesc_)
{
    // ------------------------------ 頂点シェーダーを読み込み -----------------------------
    OrcaExecuteDebug(
        if (!std::filesystem::exists(ShaderDesc_.mVsFileName))
            Orca_Assert("頂点シェーダーファイルを開けません");
    );
    Shader::ShaderBuilder vsBuilder{};
    vsBuilder.Build(ShaderDesc_.mVsFileName.c_str(), Shader::ShaderStage::VS);

    // ----------------------------- ピクセルシェーダーを読み込み ----------------------------
    OrcaExecuteDebug(
        if (!std::filesystem::exists(ShaderDesc_.mPsFileName))
            Orca_Assert("ピクセルシェーダーファイルを開けません");
    );
    Shader::ShaderBuilder psBuilder{};
    psBuilder.Build(ShaderDesc_.mPsFileName.c_str(), Shader::ShaderStage::PS);

    // ------------------------------ ルートパラメーターを取得 -----------------------------
    std::vector<D3D12_ROOT_PARAMETER> rootParameters{};
    SamplerInfo samplersInfo{};
    DescriptorRanges descriptorRanges{};

    // ディスクリプタレンジを取得するためのラムダ式
    auto getDescriptorRange = [&](OrcaComPtr(ID3D12ShaderReflection) pReflector, UINT BoundsResource_, Shader::ShaderStage ShaderStage_)
    {
        for (UINT i = 0; i < BoundsResource_; ++i)
        {
            D3D12_SHADER_INPUT_BIND_DESC bindDesc;
            HRESULT hr = pReflector->GetResourceBindingDesc(i, &bindDesc);
            OrcaDebug::GraphicsLog("リソースの情報を取得", hr);
            auto descriptorRange = Shader::ReflectionHelpers::GetDescriptorRange(bindDesc);
            // 取得した情報が静的サンプラーだった場合専用の処理に分岐する
            if (descriptorRange.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
            {
                samplersInfo.insert(std::make_pair(bindDesc.Name, descriptorRange.BaseShaderRegister));
                continue;
            }
            std::tuple tuple = { ShaderStage_,descriptorRange };
            descriptorRanges.insert(std::make_pair(bindDesc.Name, tuple));
        }
    };

    getDescriptorRange(vsBuilder.GetReflector(), vsBuilder.GetShaderDesc().BoundResources, Shader::ShaderStage::VS);
    getDescriptorRange(psBuilder.GetReflector(), psBuilder.GetShaderDesc().BoundResources, Shader::ShaderStage::PS);

    // ------------------------------ ルートパラメーターを作成 -----------------------------
    for (auto it = descriptorRanges.begin(); it != descriptorRanges.end();)
    {
        // ----------------------------- 名前ごとにアクセス -----------------------------
        const UINT counts = static_cast<UINT>(descriptorRanges.count(it->first));
        {
            //auto nameIt = descriptorRanges.find(it->first);
            //UINT shaderStage{};
            //for (UINT i = 0; i < counts; ++i)
            //{
            //    const  auto& stage = std::get<0>(nameIt->second);
            //    shaderStage |= stage;
            //    ++nameIt;
            //}
        }

        auto& range = std::get<1>(it->second);
        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// パラメータ種別
        //param.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(shaderStage); // どのシェーダーから利用可能か
        // TODO 一旦アクセス指定をオールにしているが個別設定のやり方を模索すること
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// どのシェーダーから利用可能か
        param.DescriptorTable.NumDescriptorRanges = 1;					// ディスクリプタレンジ数
        param.DescriptorTable.pDescriptorRanges = &range;	            // ディスクリプタレンジのアドレス
        rootParameters.emplace_back(param);
        // 重複している分だけイテレータを進める
        for (UINT c = 0; c < counts; ++c)
        {
            ++it;
        }
    }

    // ------------------------ ここ以下のシェーダーは読み込みに失敗してもいいよ -----------------------


    // ----------------------------- インプットレイアウトを作成 -----------------------------
    auto inputElementDescs = InputLayout::InputLayout::CreateInputElementDesc(vsBuilder);
    UINT inputElementDescCount = static_cast<UINT>(inputElementDescs.size());

    // ------------------------------ サンプラーステートを取得 -----------------------------
    auto samplerStates = PipelineObject::SamplerStates::GetStaticSamplerForReflection(samplersInfo);

    // ------------------------------ ルートシグネチャの設定 ------------------------------
    D3D12_ROOT_SIGNATURE_DESC desc{};
    desc.NumParameters = static_cast<UINT>(rootParameters.size());
    desc.pParameters = rootParameters.data();
    desc.NumStaticSamplers = static_cast<UINT>(samplerStates.size());
    desc.pStaticSamplers = samplerStates.data();
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;



    Microsoft::WRL::ComPtr<ID3DBlob> pBlob{};
    Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{};
    // シリアライズ
    auto hr = D3D12SerializeRootSignature(&desc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        pBlob.GetAddressOf(),
        pErrorBlob.GetAddressOf()
    );
    OrcaDebug::GraphicsLog("ルートシグネチャをシリアライズ", hr);

    // ルートシグネチャを生成
    hr = pDevice_->CreateRootSignature(
        0,
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        IID_PPV_ARGS(mpRootSignature.GetAddressOf())
    );

    mpRootSignature->SetName(L"Graphics");
    OrcaDebug::GraphicsLog("ルートシグネチャを生成", hr);

    // ----------------------------- ステートオブジェクトを作成 -----------------------------
  // ブレンドステートを取得
    auto blendDesc = PipelineObject::BlendStates::GetBlendDesc(ShaderDesc_);
    // ラスタライザステートを取得
    auto rasterizerState = PipelineObject::RasterizerStates::GetRasterizerDesc(ShaderDesc_);
    // デプスステンシルステートを取得
    auto depthStencilState = PipelineObject::DepthStencilStates::GetDepthStencilState(ShaderDesc_);



    // グラフィックスパイプラインオブジェクトを生成
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descGps{};
    descGps.InputLayout = { inputElementDescs.data(),inputElementDescCount };
    descGps.pRootSignature = mpRootSignature.Get();
    descGps.VS = { vsBuilder.GetBlob()->GetBufferPointer(),vsBuilder.GetBlob()->GetBufferSize() };
    descGps.PS = { psBuilder.GetBlob()->GetBufferPointer(),psBuilder.GetBlob()->GetBufferSize() };
    descGps.RasterizerState = rasterizerState;
    descGps.BlendState = blendDesc;
    descGps.DepthStencilState = depthStencilState;
    descGps.SampleMask = UINT_MAX;
    descGps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    descGps.NumRenderTargets = 1;
    descGps.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    descGps.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    descGps.SampleDesc = { 1,0 };

    // パイプラインステートを設定
    hr = pDevice_->CreateGraphicsPipelineState(
        &descGps, IID_PPV_ARGS(mpPipelineState.GetAddressOf()));

    OrcaDebug::GraphicsLog("パイプラインステートを作成", hr);

    for (const auto& aaa : inputElementDescs)
    {
        delete[] aaa.SemanticName;
    }
}

void OrcaGraphics::RenderPipeline::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    pCmdList_->SetGraphicsRootSignature(mpRootSignature.Get());
    pCmdList_->SetPipelineState(mpPipelineState.Get());
}

void OrcaGraphics::RenderPipeline::CreateRootParameter(const Shader::ShaderBuilder& ShaderBuilder_, std::vector<D3D12_ROOT_PARAMETER>& RootParameters_, DescriptorRanges& DescriptorRanges_, SamplerInfo& SamplersInfo_) const
{
    // ----------------------------- ディスクリプタレンジを生成 -----------------------------
    CreateDescriptorRange(ShaderBuilder_, DescriptorRanges_, SamplersInfo_);

    // ------------------------------ ルートパラメーターを作成 -----------------------------
    for (auto it = DescriptorRanges_.begin(); it != DescriptorRanges_.end();)
    {
        // ----------------------------- 名前ごとにアクセス -----------------------------
        const UINT counts = static_cast<UINT>(DescriptorRanges_.count(it->first));
        {
            //auto nameIt = descriptorRanges.find(it->first);
            //UINT shaderStage{};
            //for (UINT i = 0; i < counts; ++i)
            //{
            //    const  auto& stage = std::get<0>(nameIt->second);
            //    shaderStage |= stage;
            //    ++nameIt;
            //}
        }

        auto& range = std::get<1>(it->second);
        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// パラメータ種別
        //param.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(shaderStage); // どのシェーダーから利用可能か
        // TODO 一旦アクセス指定をオールにしているが個別設定のやり方を模索すること
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// どのシェーダーから利用可能か
        param.DescriptorTable.NumDescriptorRanges = 1;					// ディスクリプタレンジ数
        param.DescriptorTable.pDescriptorRanges = &range;	            // ディスクリプタレンジのアドレス
        RootParameters_.emplace_back(param);
        // 重複している分だけイテレータを進める
        for (UINT c = 0; c < counts; ++c)
        {
            ++it;
        }
    }
}

void OrcaGraphics::RenderPipeline::CreateDescriptorRange(const Shader::ShaderBuilder& ShaderBuilder_,
    DescriptorRanges& DescriptorRanges_, SamplerInfo& SamplerInfo_)
{
    // ----------------------------- ディスクリプタレンジを作成 -----------------------------
       // リフレクターから情報を取得
    const D3D12_SHADER_DESC shaderDesc = ShaderBuilder_.GetShaderDesc();
    const auto pReflector = ShaderBuilder_.GetReflector();

    for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
    {
        D3D12_SHADER_INPUT_BIND_DESC bindDesc;
        HRESULT hr = pReflector->GetResourceBindingDesc(i, &bindDesc);
        OrcaDebug::GraphicsLog("リソースの情報を取得", hr);
        auto descriptorRange = Shader::ReflectionHelpers::GetDescriptorRange(bindDesc);
        // 取得した情報が静的サンプラーだった場合専用の処理に分岐する
        if (descriptorRange.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
        {
            SamplerInfo_.insert(std::make_pair(bindDesc.Name, descriptorRange.BaseShaderRegister));
            continue;
        }
        std::tuple tuple = { ShaderBuilder_.GetShaderStage(),descriptorRange };
        DescriptorRanges_.insert(std::make_pair(bindDesc.Name, tuple));
    }
}
