#include"ShaderBuilder.h"
#include"GraphicsLogger.h"
#include <d3dcompiler.h>
#include<filesystem>

void OrcaGraphics::Shader::ShaderBuilder::Build(const wchar_t* FileName_,
    ShaderStage ShaderStage_)
{
    // --------------------------------- 変数を初期化 --------------------------------
    {
        mShaderStage = ShaderStage_;
        mFileName = FileName_;
    }
    // -------------------------------- シェーダーを生成 -------------------------------
    auto hr = D3DReadFileToBlob(FileName_, mpBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("シェーダーをBlobにロード", hr);
    
    // ------------------------------ リフレクション情報を生成 -----------------------------
    hr = D3DReflect(mpBlob->GetBufferPointer(), mpBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &mpReflector);
    OrcaDebug::GraphicsLog("リフレクション情報を取得", hr);
    // シェーダーの詳細情報を取得
    hr = mpReflector->GetDesc(&mShaderDesc);
    OrcaDebug::GraphicsLog("D3D12_SHADER_DESCを取得", hr);
}

OrcaComPtr(ID3D12ShaderReflection) OrcaGraphics::Shader::ShaderBuilder::GetReflector()const
{
    return mpReflector;
}

D3D12_SHADER_DESC OrcaGraphics::Shader::ShaderBuilder::GetShaderDesc() const
{
    return mShaderDesc;
}

OrcaGraphics::Shader::ShaderStage OrcaGraphics::Shader::ShaderBuilder::GetShaderStage() const
{
    return mShaderStage;
}

Microsoft::WRL::ComPtr<ID3DBlob> OrcaGraphics::Shader::ShaderBuilder::GetBlob() const
{
    return mpBlob;
}
