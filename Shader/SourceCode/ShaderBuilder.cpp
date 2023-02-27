#include"ShaderBuilder.h"
#include"GraphicsLogger.h"
#include <d3dcompiler.h>
#include<filesystem>

void Graphics::Shader::ShaderBuilder::Build(const wchar_t* FileName_,
    ShaderStage ShaderStage_)
{
    // --------------------------------- �ϐ��������� --------------------------------
    {
        mShaderStage = ShaderStage_;
        mFileName = FileName_;
    }
    // -------------------------------- �V�F�[�_�[�𐶐� -------------------------------
    auto hr = D3DReadFileToBlob(FileName_, mpBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("�V�F�[�_�[��Blob�Ƀ��[�h", hr);
    
    // ------------------------------ ���t���N�V�������𐶐� -----------------------------
    hr = D3DReflect(mpBlob->GetBufferPointer(), mpBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &mpReflector);
    OrcaDebug::GraphicsLog("���t���N�V���������擾", hr);
    // �V�F�[�_�[�̏ڍ׏����擾
    hr = mpReflector->GetDesc(&mShaderDesc);
    OrcaDebug::GraphicsLog("D3D12_SHADER_DESC���擾", hr);
}

OrcaComPtr(ID3D12ShaderReflection) Graphics::Shader::ShaderBuilder::GetReflector()const
{
    return mpReflector;
}

D3D12_SHADER_DESC Graphics::Shader::ShaderBuilder::GetShaderDesc() const
{
    return mShaderDesc;
}

Graphics::Shader::ShaderStage Graphics::Shader::ShaderBuilder::GetShaderStage() const
{
    return mShaderStage;
}

Microsoft::WRL::ComPtr<ID3DBlob> Graphics::Shader::ShaderBuilder::GetBlob() const
{
    return mpBlob;
}
