#pragma once
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include<vector>
#include <wrl.h>

namespace OrcaGraphics
{
    namespace Shader
    {
        class ReflectionHelpers
        {
            // ----------------------------- ���J��N���X ----------------------------
            friend class ShaderReflection;

            // --------------------- �V�F�[�_�[�����󂯎�邽�߂̃w���p�[�֐� ---------------------

            // �C���v�b�g���C�A�E�g�̃t�H�[�}�b�g���擾
            static DXGI_FORMAT GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);

            // �V�F�[�_�[���t���N�V�����̃��\�[�X�̎�ނ��擾
            static D3D12_DESCRIPTOR_RANGE_TYPE GetDescriptorRangeType(D3D_SHADER_INPUT_TYPE Type_, D3D_SRV_DIMENSION Dimension_);
            static std::vector< D3D12_DESCRIPTOR_RANGE> GetDescriptorRanges(Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector_,
                UINT BoundsResource_);
        };
    }
}
