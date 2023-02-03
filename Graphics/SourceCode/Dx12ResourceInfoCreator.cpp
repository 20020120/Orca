#include"pch.h"
#include"Dx12ResourceInfoCreator.h"
#include"OrcaException.h"
#include"GraphicsLogger.h"

#include <d3d12shader.h>

void OrcaGraphics::Resource::Dx12ResourceInfoCreator::AddResourceInfo(const Shader::ShaderType& ShaderType_,
                                                        Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector_,
    uint32_t MaxResources_)
{
    HRESULT hr;
    // ------------------------------ ���t���N�V���������擾 -----------------------------
    for (uint32_t i = 0; i < MaxResources_; ++i)
    {
        D3D12_SHADER_INPUT_BIND_DESC desc;
        hr = pReflector_->GetResourceBindingDesc(i, &desc);
        OrcaDebug::GraphicsLog("���t���N�V���������擾", hr);

        Dx12ResourceInfo info;
        info.mName = desc.Name;
        info.mType = GetResourceType(desc.Type);

        // TODO : �������̗v�f�̂��߂�Continue�ł���悤�ɂ���
        if (info.mType == ResourceType::Sampler) // �_�C�i�~�b�N�T���v���[�������ɕK�v
            continue;

        info.mRootIndex = i;
        // �萔�o�b�t�@�̂݃T�C�Y���擾
        if (info.mType == ResourceType::ConstantBuffer)
        {
            const auto cbInfo = pReflector_->GetConstantBufferByIndex(i);
            D3D12_SHADER_BUFFER_DESC bufferDesc;
            hr = cbInfo->GetDesc(&bufferDesc);
            OrcaDebug::GraphicsLog("���t���N�V���������擾", hr);
            info.mBufferSize = bufferDesc.Size;
        }
        else
            info.mBufferSize = -1;

        // --------------------------------- �v�f��ǉ� ---------------------------------
        mHolder[ShaderType_][info.mName] = info;
    }
}

const OrcaGraphics::Resource::Dx12ResourceInfoCreator::ResourcesType& OrcaGraphics::Resource::Dx12ResourceInfoCreator::GetResourceMap(const Shader::ShaderType& ShaderType_)
{
    return mHolder[ShaderType_];
}

OrcaGraphics::Resource::ResourceType OrcaGraphics::Resource::Dx12ResourceInfoCreator::GetResourceType(
    const D3D_SHADER_INPUT_TYPE& InputType_)
{
    switch (InputType_)
    {
    case D3D_SIT_CBUFFER:
        return Resource::ResourceType::ConstantBuffer;
    case D3D_SIT_TBUFFER: break;
    case D3D_SIT_TEXTURE:
        return Resource::ResourceType::Texture;
    case D3D_SIT_SAMPLER:
        return Resource::ResourceType::Sampler;
    case D3D_SIT_STRUCTURED: break;
    case D3D_SIT_BYTEADDRESS: break;
    case D3D_SIT_RTACCELERATIONSTRUCTURE:

    case D3D_SIT_UAV_RWSTRUCTURED:
    case D3D_SIT_UAV_RWTYPED: 
    case D3D_SIT_UAV_RWBYTEADDRESS:
    case D3D_SIT_UAV_APPEND_STRUCTURED:
    case D3D_SIT_UAV_CONSUME_STRUCTURED: 
    case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
    case D3D_SIT_UAV_FEEDBACKTEXTURE:
        return Resource::ResourceType::UAV;
    }

    Orca_Unimplemented;
    return Resource::ResourceType::Unknown;
}
