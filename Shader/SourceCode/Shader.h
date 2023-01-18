#pragma once
#include <d3d12.h>
#include<wrl.h>
#include<map>
#include<string>
#include"MacroMinmam.h"
// -------------------------------- �O���錾 -------------------------------
struct ID3D12GraphicsCommandList;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Device;
// �V�F�[�_�[���N���X
namespace OrcaGraphics
{
    namespace Shader
    {
        class Shader
        {
        public:
            void Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const char* VsPath_, const char* PsPath_);
            void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_);

        private:
            Microsoft::WRL::ComPtr<ID3D12RootSignature> mpRootSignature{};
            Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPso{};

        public:
            // ���\�[�X�o�C���h���ɕK�v�ȃp�����[�^�[�����f���쐬���ɐ����ł���悤�ɂ��Ă���
            using RootParamIndexes = std::map<std::string, UINT>;

            OrcaNodiscard RootParamIndexes GetRootParamIndexes()const;
        private:
            RootParamIndexes mRootParamIndexes{};    // ���\�[�X�̖��O�ƃ��[�g�p�����[�^�[�̃C���f�b�N�X
        };
    }
}
