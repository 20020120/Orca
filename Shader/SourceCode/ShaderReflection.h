#pragma once
#include <d3d12.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include<vector>
#include<wrl.h>

namespace OrcaGraphics
{
    namespace Shader
    {
        // �V�F�[�_�[���t���N�V�����p�N���X(�V�F�[�_�[�n�̃N���X�ȊO�ɂ͉B���Ă�������)
        class ShaderReflection
        {
            // ----------------------------- ���J��N���X ----------------------------
            friend class Shader;

            enum   ShaderStage
            {
                Vertex,
                Domain,
                Hull,
                Geometry,
                Pixel,
                Compute
            };

            // -------------------- �V�F�[�_�[���烊�t���N�V��������Ԃ��\���� -------------------
            struct ReflectionData
            {
                ReflectionData() = default;
                ReflectionData(const ReflectionData& RHS_) = default;
                ReflectionData& operator=(const ReflectionData&) = default;
                ReflectionData(ReflectionData&& RHS_)noexcept;
                ~ReflectionData();

                std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElementDescs{};   // �C���v�b�g���C�A�E�g
                UINT mNumInputElements{};   // �C���v�b�g���C�A�E�g�̐�
                std::vector<D3D12_DESCRIPTOR_RANGE> mDescriptorRanges{};    // �f�B�X�N���v�^�����W
                std::vector<ShaderStage> mDescriptorRangeShaderStage{};  // �f�B�X�N���v�^�����W�̃V�F�[�_�[�X�e�[�W

            };

            
            // �V�F�[�_�[���t���N�V���������s����
            static ReflectionData Execute(Microsoft::WRL::ComPtr<ID3DBlob> pBlobVs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobPs_,
                Microsoft::WRL::ComPtr<ID3DBlob> pBlobGs_ = nullptr, Microsoft::WRL::ComPtr<ID3DBlob> pBlobDs_ = nullptr,
                Microsoft::WRL::ComPtr<ID3DBlob> pBlobHs_ = nullptr);
        };
    }
}
