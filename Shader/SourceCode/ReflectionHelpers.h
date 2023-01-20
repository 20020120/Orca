#pragma once
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcommon.h>
#include <dxgiformat.h>

namespace OrcaGraphics
{
    // ---------------------------------- �O���錾 ---------------------------------
    namespace InputLayout
    {
        class InputLayout;
    }
    class RenderPipeline;

    namespace Shader
    {
        class ReflectionHelpers
        {
            // ----------------------------- ���J��N���X ----------------------------
            friend InputLayout::InputLayout;
            friend RenderPipeline;

            // --------------------- �V�F�[�_�[�����󂯎�邽�߂̃w���p�[�֐� ---------------------
            // �C���v�b�g���C�A�E�g�̃t�H�[�}�b�g���擾
            static DXGI_FORMAT GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);

            // �V�F�[�_�[���t���N�V�����̃��\�[�X�̎�ނ��擾
            static D3D12_DESCRIPTOR_RANGE_TYPE GetDescriptorRangeType(D3D_SHADER_INPUT_TYPE Type_, D3D_SRV_DIMENSION Dimension_);
            static D3D12_DESCRIPTOR_RANGE GetDescriptorRange(D3D12_SHADER_INPUT_BIND_DESC BindDesc_);
        };
    }
}
