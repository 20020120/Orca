#include"ReflectionHelpers.h"

#include "GraphicsLogger.h"
#include"OrcaException.h"


DXGI_FORMAT OrcaGraphics::Shader::ReflectionHelpers::GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask)
{
    if (mask <= 1)
    {
        // x
        switch (type)
        {
        case D3D_REGISTER_COMPONENT_UNKNOWN:
            return DXGI_FORMAT_UNKNOWN;
        case D3D10_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32_UINT;
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32_FLOAT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32_SINT;
        }
    }
    if (mask <= 3)
    {
        // xy
        switch (type)
        {
        case D3D_REGISTER_COMPONENT_UNKNOWN:
            return DXGI_FORMAT_UNKNOWN;
        case D3D10_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32_UINT;
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32_FLOAT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32_SINT;
        }
    }
    if (mask <= 7)
    {
        // xyz
        switch (type)
        {
        case D3D_REGISTER_COMPONENT_UNKNOWN:
            return DXGI_FORMAT_UNKNOWN;
        case D3D10_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32B32_UINT;
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32B32_SINT;
        }
    }
    if (mask <= 15)
    {
        // xyzw
        switch (type)
        {
        case D3D_REGISTER_COMPONENT_UNKNOWN:
            return DXGI_FORMAT_UNKNOWN;
        case D3D10_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32B32A32_UINT;
        case D3D10_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32B32A32_SINT;
        }
    }
    return DXGI_FORMAT_UNKNOWN;
}


D3D12_DESCRIPTOR_RANGE_TYPE OrcaGraphics::Shader::ReflectionHelpers::GetDescriptorRangeType(D3D_SHADER_INPUT_TYPE Type_, D3D_SRV_DIMENSION Dimension_)
{
    switch (Type_) {
    case D3D_SIT_CBUFFER:
        return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

    case D3D_SIT_TBUFFER:break;
    case D3D_SIT_TEXTURE: break;

    case D3D_SIT_SAMPLER:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
    case D3D_SIT_STRUCTURED: break;
    case D3D_SIT_BYTEADDRESS: break;
    case D3D_SIT_RTACCELERATIONSTRUCTURE: break;

    case D3D_SIT_UAV_RWTYPED:                   [[fallthrough]];
    case D3D_SIT_UAV_RWSTRUCTURED:              [[fallthrough]];
    case D3D_SIT_UAV_RWBYTEADDRESS:             [[fallthrough]];
    case D3D_SIT_UAV_APPEND_STRUCTURED:         [[fallthrough]];
    case D3D_SIT_UAV_CONSUME_STRUCTURED:        [[fallthrough]];
    case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER: [[fallthrough]];
    case D3D_SIT_UAV_FEEDBACKTEXTURE:
        return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    }

    // �����ŃN���b�V�������烊�t���N�V�����̖��ݒ�̍��ڂ�����̂Őݒ肵�Ă�������
    Orca_Unimplemented

    return D3D12_DESCRIPTOR_RANGE_TYPE();
}

std::vector<D3D12_DESCRIPTOR_RANGE> OrcaGraphics::Shader::ReflectionHelpers::GetDescriptorRanges(
    Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector_, UINT BoundsResource_)
{
    std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRanges{};
    // ���\�[�X�̃f�[�^���擾
    for (UINT i = 0; i < BoundsResource_; i++)
    {
        D3D12_SHADER_INPUT_BIND_DESC bindDesc;
        const auto hr = pReflector_->GetResourceBindingDesc(i, &bindDesc);
        OrcaDebug::GraphicsLog("���\�[�X�̏����擾", hr);
        {
            // �f�B�X�N���v�^�����W���쐬
            D3D12_DESCRIPTOR_RANGE descriptorRange;
            // ���\�[�X�̎�ނ𔻕�
            descriptorRange.RangeType = ReflectionHelpers::GetDescriptorRangeType(bindDesc.Type, bindDesc.Dimension);
            descriptorRange.NumDescriptors = 1;
            // �X���b�g�𔻕�
            descriptorRange.BaseShaderRegister = bindDesc.BindPoint;
            descriptorRange.RegisterSpace = bindDesc.Space;
            descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            // �f�B�X�N���v�^�����W��o�^
            descriptorRanges.emplace_back(descriptorRange);
        }
    }

    return descriptorRanges;
}
