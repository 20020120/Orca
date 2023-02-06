#pragma once
//#include <d3d12.h>
#include"GraphicsMacro.h"
#include<string>

struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

namespace OrcaGraphics
{
    // ---------------------------------- �O���錾 ---------------------------------
    class DescriptorPool;
    class DescriptorHandle;

    namespace Resource
    {
        // ---------------------------------- �`��p�̃��\�[�X�̊��N���X ---------------------------------
        class Dx12Resource
        {
        public:
            // ���ʂ̏�����
            Dx12Resource(std::string Name_, DescriptorPool* pDescriptorPool_, UINT RootParamIndex);
            virtual ~Dx12Resource();

            void Bind(OrcaComPtr(ID3D12GraphicsCommandList) pCmdList_) const; // GPU�Ƀo�C���h
            std::string GetName()const;
        private:
            UINT mRootParamIndex{}; // ���[�g�p�����[�^�[�̔ԍ�
        protected:
            DescriptorHandle* mpHandle{};
            DescriptorPool* mpPool{};
            OrcaComPtr(ID3D12Resource) mpResource {};
            std::string mName{};
        };
    }
}

