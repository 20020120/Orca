// ReSharper disable CppClangTidyClangDiagnosticPessimizingMove
#include "Shader.h"
#include "GraphicsLogger.h"
#include"ShaderReflection.h"
#include"SamplerStates.h"
#include<d3d12.h>
#include <d3dcompiler.h>

#include "SamplerStates.h"
#include"BlendStates.h"
#include"DepthStencilStates.h"
#include"RasterizerStates.h"

void OrcaGraphics::Shader::Shader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const char* VsPath_, const char* PsPath_)
{
    HRESULT hr{};
    // 頂点シェーダー読み込み
    Microsoft::WRL::ComPtr<ID3DBlob> pVsBlob{};
    hr = D3DReadFileToBlob(L"../Resource/Shader/ObjVs.cso", pVsBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("頂点シェーダーを読み込み", hr);

    // ピクセルシェーダー読み込み
    Microsoft::WRL::ComPtr<ID3DBlob> pPsBlob{};
    hr = D3DReadFileToBlob(L"../Resource/Shader/ObjPs.cso", pPsBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("ピクセルシェーダーを読み込み", hr);

    // シェーダーからリフレクションを使って情報を取得
    auto reflectionData = ShaderReflection::Execute(pVsBlob,pPsBlob);

    // ------------------------------ ルートパラメーターを設定 -----------------------------
    std::vector<D3D12_ROOT_PARAMETER> rootParameters;
    for (auto& [shaderStage, range] : reflectionData.mDescriptorRanges)
    {
        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// パラメータ種別
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				// どのシェーダーから利用可能か
        param.DescriptorTable.NumDescriptorRanges = 1;							// ディスクリプタレンジ数
        param.DescriptorTable.pDescriptorRanges = &range;	                    // ディスクリプタレンジのアドレス
        rootParameters.emplace_back(param);
    }


    // スタティックサンプラーを取得
    auto staticSampler = PipelineObject::SamplerStates::GetStaticSampler();

    // ルートシグネチャの設定
    D3D12_ROOT_SIGNATURE_DESC desc{};
    desc.NumParameters = static_cast<UINT>(rootParameters.size());
    desc.pParameters = rootParameters.data();
    desc.NumStaticSamplers = 1;
    desc.pStaticSamplers = &staticSampler;
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob{};
    Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{};
    // シリアライズ
    hr = D3D12SerializeRootSignature(&desc,
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


    // ラスタライザーステートの設定
    auto rasterizerState = PipelineObject::RasterizerStates::GetSampleRasterizerState();
    // デプスステンシルステートを設定
    auto depthStencilState = PipelineObject::DepthStencilStates::GetSampleDepthStencilState();
    // ブレンドステートを取得
    auto blendState = PipelineObject::BlendStates::GetSampleBlendState();


    // グラフィックスパイプラインオブジェクトを生成
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descGps{};
    descGps.InputLayout = { reflectionData.mInputElementDescs.data(), reflectionData.mNumInputElements };
    descGps.pRootSignature = mpRootSignature.Get();
    descGps.VS = { pVsBlob->GetBufferPointer(),pVsBlob->GetBufferSize() };
    descGps.PS = { pPsBlob->GetBufferPointer(),pPsBlob->GetBufferSize() };
    descGps.RasterizerState = rasterizerState;
    descGps.BlendState = blendState;
    descGps.DepthStencilState = depthStencilState;
    descGps.SampleMask = UINT_MAX;
    descGps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    descGps.NumRenderTargets = 1;
    descGps.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    descGps.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    descGps.SampleDesc = { 1,0 };

    // パイプラインステートを設定
    hr = pDevice_->CreateGraphicsPipelineState(
        &descGps, IID_PPV_ARGS(mpPso.GetAddressOf()));

    OrcaDebug::GraphicsLog("パイプラインステートを作成", hr);
}

void OrcaGraphics::Shader::Shader::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    pCmdList_->SetGraphicsRootSignature(mpRootSignature.Get());
    pCmdList_->SetPipelineState(mpPso.Get());
}
