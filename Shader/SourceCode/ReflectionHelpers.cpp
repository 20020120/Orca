#include"ReflectionHelpers.h"
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
    case D3D_SIT_TEXTURE:
        // �o�b�t�@�ȊO�̓e�N�X�`���Ƃ��Ĉ���
        if(Dimension_!=D3D_SRV_DIMENSION_BUFFER)
        {
            return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        }
        // �V�F�[�_�[���\�[�X�r���[�̓o�b�t�@�̉\�������邩���U��������
        break;
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

D3D12_DESCRIPTOR_RANGE OrcaGraphics::Shader::ReflectionHelpers::GetDescriptorRange(D3D12_SHADER_INPUT_BIND_DESC BindDesc_)
{
    // �f�B�X�N���v�^�����W���쐬
    D3D12_DESCRIPTOR_RANGE descriptorRange;
    // ���\�[�X�̎�ނ𔻕�
    descriptorRange.RangeType = GetDescriptorRangeType(BindDesc_.Type, BindDesc_.Dimension);
    descriptorRange.NumDescriptors = 1;
    // �X���b�g�𔻕�
    descriptorRange.BaseShaderRegister = BindDesc_.BindPoint;
    descriptorRange.RegisterSpace = BindDesc_.Space;
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    return descriptorRange;
}