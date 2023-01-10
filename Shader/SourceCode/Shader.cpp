#include "Shader.h"
#include "GraphicsLogger.h"
#include<d3d12.h>
#include <d3dcompiler.h>


void OrcaGraphics::Shader::Shader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const char* VsPath_, const char* PsPath_)
{
    // ------------------------------ ルートシグネチャの作成 ------------------------------
    D3D12_DESCRIPTOR_RANGE range[2]{};
    // b0
    range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    range[0].NumDescriptors = 1;
    range[0].BaseShaderRegister = 0;
    range[0].RegisterSpace = 0;
    range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // b1
    range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;	// レンジ種別
    range[1].NumDescriptors = 1;							// ディスクリプタ数
    range[1].BaseShaderRegister = 1;						// 先頭レジスタ番号
    range[1].RegisterSpace = 0;								// つじつまを合わせるためのスペース
    range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


    // ルートパラメータの設定
    D3D12_ROOT_PARAMETER param[2]{};
    param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// パラメータ種別
    param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				    // どのシェーダーから利用可能か
    param[0].DescriptorTable.NumDescriptorRanges = 1;							// ディスクリプタレンジ数
    param[0].DescriptorTable.pDescriptorRanges = &range[0];	                    // ディスクリプタレンジのアドレス

    param[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// パラメータ種別
    param[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;				    // どのシェーダーから利用可能か
    param[1].DescriptorTable.NumDescriptorRanges = 1;							// ディスクリプタレンジ数
    param[1].DescriptorTable.pDescriptorRanges = &range[1];	                    // ディスクリプタレンジのアドレス


    // スタティックサンプラーの設定
    D3D12_STATIC_SAMPLER_DESC descSSD{};
    descSSD.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    descSSD.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
    descSSD.MaxAnisotropy = 1;
    descSSD.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    descSSD.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    descSSD.MinLOD = -D3D12_FLOAT32_MAX;
    descSSD.MaxLOD = +D3D12_FLOAT32_MAX;
    descSSD.ShaderRegister = 0;
    descSSD.RegisterSpace = 0;
    descSSD.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


    // ルートシグネチャの設定
    D3D12_ROOT_SIGNATURE_DESC desc{};
    desc.NumParameters = _countof(param);
    desc.pParameters = param;
    desc.NumStaticSamplers = 1;
    desc.pStaticSamplers = &descSSD;
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



    // インプットレイアウトを作成
    D3D12_INPUT_ELEMENT_DESC elements[2]{};
    elements[0].SemanticName = "POSITION";
    elements[0].SemanticIndex = 0;
    elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    elements[0].InputSlot = 0;
    elements[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    elements[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    elements[0].InstanceDataStepRate = 0;

    elements[1].SemanticName = "NORMAL";
    elements[1].SemanticIndex = 0;
    elements[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    elements[1].InputSlot = 0;
    elements[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    elements[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    elements[1].InstanceDataStepRate = 0;


    // ラスタライザーステートの設定
    D3D12_RASTERIZER_DESC descRS{};
    descRS.FillMode = D3D12_FILL_MODE_SOLID;
    descRS.CullMode = D3D12_CULL_MODE_NONE;
    descRS.FrontCounterClockwise = FALSE;
    descRS.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    descRS.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    descRS.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    descRS.DepthClipEnable = FALSE;
    descRS.MultisampleEnable = FALSE;
    descRS.AntialiasedLineEnable = FALSE;
    descRS.ForcedSampleCount = 0;
    descRS.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // レンダーターゲットのブレンド設定.
    D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
        FALSE, FALSE,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL
    };


    // ブレンドステートの設定.
    D3D12_BLEND_DESC descBS;
    descBS.AlphaToCoverageEnable = FALSE;
    descBS.IndependentBlendEnable = FALSE;
    for (auto& i : descBS.RenderTarget)
    {
        i = descRTBS;
    }
    // デプスステンシルステートを設定
    D3D12_DEPTH_STENCIL_DESC descDSS{};
    descDSS.DepthEnable = TRUE;
    descDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    descDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    descDSS.StencilEnable = FALSE;



    // 頂点シェーダー読み込み
    Microsoft::WRL::ComPtr<ID3DBlob> pVsBlob{};
    hr = D3DReadFileToBlob(L"../Resource/Shader/ObjVs.cso", pVsBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("頂点シェーダーを読み込み", hr);

    // ピクセルシェーダー読み込み
    Microsoft::WRL::ComPtr<ID3DBlob> pPsBlob{};
    hr = D3DReadFileToBlob(L"../Resource/Shader/ObjPs.cso", pPsBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("ピクセルシェーダーを読み込み", hr);


    // グラフィックスパイプラインオブジェクトを生成
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descGps{};
    descGps.InputLayout = { elements, _countof(elements) };
    descGps.pRootSignature = mpRootSignature.Get();
    descGps.VS = { pVsBlob->GetBufferPointer(),pVsBlob->GetBufferSize() };
    descGps.PS = { pPsBlob->GetBufferPointer(),pPsBlob->GetBufferSize() };
    descGps.RasterizerState = descRS;
    descGps.BlendState = descBS;
    descGps.DepthStencilState = descDSS;
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
