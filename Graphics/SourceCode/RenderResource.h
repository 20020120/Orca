#pragma once
#include <d3d12.h>
#include <intsafe.h>
#include"GraphicsMacro.h"

namespace OrcaGraphics
{
    // ---------------------------------- �O���錾 ---------------------------------
    class DescriptorPool;
    class DescriptorHandle;

    // ---------------------------------- �`��p�̃��\�[�X�̊��N���X ---------------------------------
    class RenderResource
    {
    public:
        // ���ʂ̏�����
        RenderResource(DescriptorPool* pDescriptorPool_, UINT RootParamIndex);
        virtual ~RenderResource();
        void Bind(OrcaComPtr(ID3D12GraphicsCommandList) pCmdList_) const; // GPU�Ƀo�C���h

    private:
        UINT mRootParamIndex{}; // ���[�g�p�����[�^�[�̔ԍ�
    protected:
        DescriptorHandle* mpHandle{};
        DescriptorPool* mpPool{};
        OrcaComPtr(ID3D12Resource) mpResource {};
    };
}

