#pragma once
#include"GraphicsMacro.h"

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
            Dx12Resource(DescriptorPool* pDescriptorPool_);
            virtual ~Dx12Resource();

            // ------------------------------ �Q�b�^�[ -----------------------------
            [[nodiscard]] uint32_t GetDescriptorIndex()const;
            [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle()const;
        private:
            UINT mDescriptorIndex{}; // �f�B�X�N���v�^�̐�����
        protected:
            DescriptorHandle* mpHandle{};
            DescriptorPool* mpPool{};
            OrcaComPtr(ID3D12Resource) mpResource {};
        };
    }
}

