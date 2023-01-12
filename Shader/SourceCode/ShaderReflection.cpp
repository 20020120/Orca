#include "ShaderReflection.h"
#include<cassert>
#include <d3d12shader.h>
#include <d3dcompiler.h>
#include "GraphicsLogger.h"
#include"ReflectionHelpers.h"

OrcaGraphics::Shader::ShaderReflection::ReflectionData OrcaGraphics::Shader::ShaderReflection::Execute(
    const Microsoft::WRL::ComPtr<ID3DBlob> pBlobVs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobPs_,
    Microsoft::WRL::ComPtr<ID3DBlob> pBlobGs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobDs_,
    Microsoft::WRL::ComPtr<ID3DBlob> pBlobHs_)
{
    ReflectionData reflectionData;

    // -------------------------------- 頂点シェーダー --------------------------------
    if(pBlobVs_)
    {
        // シェーダーから必要な情報を取得する
        Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector;
        auto hr = D3DReflect(pBlobVs_->GetBufferPointer(), pBlobVs_->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
        OrcaDebug::GraphicsLog("頂点シェーダーのリフレクション情報を取得", hr);

        // リフレクターから情報を取得
        D3D12_SHADER_DESC shaderDesc;
        pReflector->GetDesc(&shaderDesc);

        // インプットレイアウトを取得
        std::vector<D3D12_INPUT_ELEMENT_DESC> vbElement;
        for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i) {
            D3D12_SIGNATURE_PARAMETER_DESC sigDesc;
            pReflector->GetInputParameterDesc(i, &sigDesc);

            //フォーマットを取得
            const auto format = ReflectionHelpers::GetDxgiFormat(sigDesc.ComponentType, sigDesc.Mask);

            D3D12_INPUT_ELEMENT_DESC elementDesc;
            const int semanticsLen = { static_cast<int>(std::strlen(sigDesc.SemanticName) + 1) };
            const auto pTmp = new char[semanticsLen]();
            std::memcpy(pTmp, sigDesc.SemanticName, semanticsLen);
            elementDesc.SemanticName = pTmp;
            elementDesc.SemanticIndex = 0;
            elementDesc.Format = format;
            elementDesc.InputSlot = 0;
            elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            elementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            elementDesc.InstanceDataStepRate = 0;
            reflectionData.mInputElementDescs.emplace_back(elementDesc);
        }
        // インプットレイアウトの数を設定
        reflectionData.mNumInputElements = shaderDesc.InputParameters;
        // ディスクリプタレンジを取得
        for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
        {
            D3D12_SHADER_INPUT_BIND_DESC bindDesc;
            const auto hr = pReflector->GetResourceBindingDesc(i, &bindDesc);
            OrcaDebug::GraphicsLog("リソースの情報を取得", hr);
            std::tuple tuple = { ShaderStage::Vertex,ReflectionHelpers::GetDescriptorRange(bindDesc) };
            reflectionData.mDescriptorRanges.emplace_back(tuple);
        }
    }
    // ------------------------------- ピクセルシェーダー -------------------------------
    if(pBlobPs_)
    {
        // シェーダーから必要な情報を取得する
        Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector;
        auto hr = D3DReflect(pBlobPs_->GetBufferPointer(), pBlobPs_->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
        OrcaDebug::GraphicsLog("ピクセルシェーダーのリフレクション情報を取得", hr);

        // リフレクターから情報を取得
        D3D12_SHADER_DESC shaderDesc;
        pReflector->GetDesc(&shaderDesc);
        // ディスクリプタレンジを取得
        for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
        {
            std::tuple tuple = { ShaderStage::Vertex,ReflectionHelpers::GetDescriptorRange(pReflector, i) };
            reflectionData.mDescriptorRanges.emplace_back(tuple);
        }
    }
    // -------------------------------- ドメインシェーダ -------------------------------
    if(pBlobDs_)
    {
        
    }
    // -------------------------------- ハルシェーダー --------------------------------
    if(pBlobHs_)
    {
        
    }
    // ------------------------------- ジオメトリシェーダー ------------------------------
    if(pBlobGs_)
    {
        
    }

    return std::move(reflectionData);
}


// -------------------------- シェーダーリフレクションで返す用の構造体の関数 --------------------------
OrcaGraphics::Shader::ShaderReflection::ReflectionData::ReflectionData(ReflectionData&& RHS_) noexcept
{
    *this = RHS_;
    RHS_.mInputElementDescs.clear();
}

OrcaGraphics::Shader::ShaderReflection::ReflectionData::~ReflectionData()
{
    for (const auto& aaa : mInputElementDescs)
    {
        delete[] aaa.SemanticName;
    }
}
