#pragma once
#include"ShaderDesc.h"
#include"GraphicsMacro.h"

#include<vector>
#include<map>
#include<d3d12shader.h>
#include<d3d12.h>

// ------------------------------------ �O���錾 -----------------------------------
struct ID3D12GraphicsCommandList;
struct IDxcBlob;
struct IDxcUtils;
struct ID3D12RootSignature;
struct ID3D12PipelineState;

// -------------------------------- �`��p�V�F�[�_�[�N���X --------------------------------
namespace OrcaGraphics
{
    namespace Shader
    {
        class Shader
        {
            // -------------------- �o�C���h���邽�߂̃f�[�^��A���t���N�V�����f�[�^ -------------------
            struct Data
            {
                void* m_pBin{};        // �V�F�[�_�[�̐擪�A�h���X
                size_t m_BinSize{};    // �o�C�i���̃T�C�Y
                void* m_pRef{};
                size_t m_RefSize{};
                D3D12_SHADER_DESC mShaderDesc{};
            };
            
            using DescriptorRanges = std::map<std::string, D3D12_DESCRIPTOR_RANGE>; // �f�B�X�N���v�^�̏ȗ��^
            using SamplerInfo = std::map<std::string, UINT>;   // �g���T���v���[�X�e�[�g�̏����擾����(���O/�o�C���h�X���b�g)
        public:
            // ------------------------------ ������ ------------------------------
            explicit Shader(IDxcUtils* pUtils_, const ShaderDesc& ShaderDesc_);

            // �R�}���h���Z�b�g
            void Set(ID3D12GraphicsCommandList* pCmdList_);

            void CreateRootRootSignature(IDxcUtils* pUtils_, const ShaderDesc& Desc_);   // ���[�g�V�O�l�`�����쐬
            void CreatePSO(const std::vector<D3D12_INPUT_ELEMENT_DESC>& InputElementDescs_, 
                const ShaderDesc& ShaderDesc_);   // �p�C�v���C���X�e�[�g�I�u�W�F�N�g���쐬
        private:
            // --------------------------- �e�V�F�[�_�[�̃f�[�^ --------------------------
            Data m_Vs{};
            Data m_Ps{};
            ShaderType m_ShaderType; // ���
            OrcaComPtr(ID3D12RootSignature) mpRootSignature {};
            OrcaComPtr(ID3D12PipelineState) mpPipelineState {};
        };
    }
}