#include"Shader.h"
#include"Binary.h"
#include "GraphicsLogger.h"
#include "GraphicsMacro.h"
#include "OrcaException.h"

#include"ReflectionHelpers.h"

#include "SamplerStates.h"
#include"InputLayout.h"
#include"BlendStates.h"


#include<d3d12.h>
#include<filesystem>
#include <dxcapi.h>

#include "DepthStencilStates.h"
#include "Graphics.h"
#include "RasterizerStates.h"

OrcaGraphics::Shader::Shader::Shader(IDxcUtils* pUtils_, const ShaderDesc& ShaderDesc_)
    :m_ShaderType(ShaderDesc_.m_ShaderType)
{
    {
        // ------------------------------ 頂点シェーダーを読み込み -----------------------------
        std::filesystem::path vsFileName = ShaderDesc_.m_VsFileName;
        OrcaBinary::Load(vsFileName.c_str(), &m_Vs.m_pBin, m_Vs.m_BinSize);
        OrcaBinary::Load(vsFileName.replace_extension("ref").c_str(), &m_Vs.m_pRef, m_Vs.m_RefSize);
    }
    {
        // ----------------------------- ピクセルシェーダーを読み込み ----------------------------
        std::filesystem::path psFileName = ShaderDesc_.m_PsFileName;
        OrcaBinary::Load(psFileName.c_str(), &m_Ps.m_pBin, m_Ps.m_BinSize);
        OrcaBinary::Load(psFileName.replace_extension("ref").c_str(), &m_Ps.m_pRef, m_Ps.m_RefSize);
    }

    // ------------------------------ ルートシグネチャを作成 ------------------------------
    CreateRootRootSignature(pUtils_, ShaderDesc_);


    // --------------------------------- PSOを作成 --------------------------------





}

void OrcaGraphics::Shader::Shader::Set(ID3D12GraphicsCommandList* pCmdList_)
{

}

void OrcaGraphics::Shader::Shader::CreateRootRootSignature(IDxcUtils* pUtils_, const ShaderDesc& Desc_)
{
    std::vector<D3D12_ROOT_PARAMETER> rootParameters{};
    SamplerInfo samplersInfo{};
    DescriptorRanges descriptorRanges{};
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs{};

    // ディスクリプタレンジを取得するためのラムダ式
    auto getDescriptorRange = [&](ID3D12ShaderReflection* pReflector_, UINT BoundsResource_)
    {
        for (UINT i = 0; i < BoundsResource_; ++i)
        {
            D3D12_SHADER_INPUT_BIND_DESC bindDesc;
            const HRESULT hr = pReflector_->GetResourceBindingDesc(i, &bindDesc);
            OrcaDebug::GraphicsLog("リソースの情報を取得", hr);
            auto descriptorRange = ReflectionHelpers::GetDescriptorRange(bindDesc);
            // 取得した情報が静的サンプラーだった場合専用の処理に分岐する
            if (descriptorRange.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER) {
                samplersInfo.insert(std::make_pair(bindDesc.Name, descriptorRange.BaseShaderRegister));
                continue;
            }
            descriptorRanges.try_emplace(bindDesc.Name, descriptorRange);
        }
    };

    // ----------------------------- リフレクションデータを取得 -----------------------------
    {
        ID3D12ShaderReflection* pReflection;
        const DxcBuffer reflectionData{
            m_Vs.m_pRef,
            m_Vs.m_RefSize,
            DXC_CP_ACP, };
        if (FAILED(pUtils_->CreateReflection(&reflectionData, IID_PPV_ARGS(&pReflection)))) {
            Orca_Assert("リフレクションデータの読み取りに失敗しました");
        }
        D3D12_SHADER_DESC desc;
        pReflection->GetDesc(&desc);
        getDescriptorRange(pReflection, desc.BoundResources);
        // ----------------------------- インプットレイアウトを作成 -----------------------------
        inputElementDescs = InputLayout::InputLayout::CreateInputElementDesc(&pReflection, desc);
    }
    {
        ID3D12ShaderReflection* pReflection;
        const DxcBuffer reflectionData{
            m_Ps.m_pRef,
            m_Ps.m_RefSize,
            DXC_CP_ACP, };
        if (FAILED(pUtils_->CreateReflection(&reflectionData, IID_PPV_ARGS(&pReflection)))) {
            Orca_Assert("リフレクションデータの読み取りに失敗しました");
        }
        D3D12_SHADER_DESC desc;
        pReflection->GetDesc(&desc);
        getDescriptorRange(pReflection, desc.BoundResources);
    }

    // ------------------------------ ルートパラメーターを作成 -----------------------------
    for (auto it = descriptorRanges.begin(); it != descriptorRanges.end(); ++it)
    {
        auto& range = it->second;
        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// パラメータ種別
        //param.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(shaderStage); // どのシェーダーから利用可能か
        // TODO 一旦アクセス指定をオールにしているが個別設定のやり方を模索すること
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// どのシェーダーから利用可能か
        param.DescriptorTable.NumDescriptorRanges = 1;					// ディスクリプタレンジ数
        param.DescriptorTable.pDescriptorRanges = &range;	            // ディスクリプタレンジのアドレス
        rootParameters.emplace_back(param);
    }
    {
        // ------------------------------ サンプラーステートを取得 -----------------------------
        auto samplerStates = PipelineObject::SamplerStates::GetStaticSamplerForReflection(samplersInfo);

        // ------------------------------ ルートシグネチャの設定 ------------------------------
        D3D12_ROOT_SIGNATURE_DESC desc{};
        desc.NumParameters = static_cast<UINT>(rootParameters.size());
        desc.pParameters = rootParameters.data();
        desc.NumStaticSamplers = static_cast<UINT>(samplerStates.size());
        desc.pStaticSamplers = samplerStates.data();
        desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED| D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        // ---------------------------- ルートシグネチャをシリアライズ ----------------------------
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob{};
        Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{};
        auto hr = D3D12SerializeRootSignature(&desc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            pBlob.GetAddressOf(),
            pErrorBlob.GetAddressOf()
        );
        OrcaDebug::GraphicsLog("ルートシグネチャをシリアライズ", hr);

        auto pDevice = OrcaGraphics::Graphics::GetDevice();
        // ルートシグネチャを生成
        hr = pDevice->CreateRootSignature(
            0,
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            IID_PPV_ARGS(mpRootSignature.GetAddressOf())
        );
        mpRootSignature->SetName(L"Graphics");
        OrcaDebug::GraphicsLog("ルートシグネチャを生成", hr);
    }
    // ----------------------------- ステートオブジェクトを作成 -----------------------------
    // ブレンドステートを取得
    auto blendDesc = PipelineObject::BlendStates::GetBlendDesc(Desc_);
    // ラスタライザステートを取得
    auto rasterizerState = PipelineObject::RasterizerStates::GetRasterizerDesc(Desc_);
    // デプスステンシルステートを取得
    auto depthStencilState = PipelineObject::DepthStencilStates::GetDepthStencilState(Desc_);

    // グラフィックスパイプラインオブジェクトを生成
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descGps{};
    descGps.InputLayout = { inputElementDescs.data(),static_cast<UINT>(inputElementDescs.size()) };
    descGps.pRootSignature = mpRootSignature.Get();
    descGps.VS = { m_Vs.m_pBin,m_Vs.m_BinSize };
    descGps.PS = { m_Ps.m_pBin,m_Ps.m_BinSize };
    descGps.RasterizerState = rasterizerState;
    descGps.BlendState = blendDesc;
    descGps.DepthStencilState = depthStencilState;
    descGps.SampleMask = UINT_MAX;
    descGps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    descGps.NumRenderTargets = 1;
    descGps.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    descGps.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    descGps.SampleDesc = { 1,0 };

    // ----------------------------- パイプラインステートを設定 -----------------------------
    auto pDevice = Graphics::GetDevice();
    auto hr = pDevice->CreateGraphicsPipelineState(
        &descGps, IID_PPV_ARGS(mpPipelineState.GetAddressOf()));
    OrcaDebug::GraphicsLog("パイプラインステートを作成", hr);

    for (const auto& aaa : inputElementDescs)
    {
        delete[] aaa.SemanticName;
    }
}
