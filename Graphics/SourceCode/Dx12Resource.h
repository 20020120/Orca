#pragma once
#include"GraphicsMacro.h"

struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

namespace OrcaGraphics
{
    // ---------------------------------- 前方宣言 ---------------------------------
    class DescriptorPool;
    class DescriptorHandle;

    namespace Resource
    {
        // ---------------------------------- 描画用のリソースの基底クラス ---------------------------------
        class Dx12Resource
        {
        public:
            // 共通の初期化
            Dx12Resource(DescriptorPool* pDescriptorPool_);
            virtual ~Dx12Resource();

            // ------------------------------ ゲッター -----------------------------
            [[nodiscard]] uint32_t GetDescriptorIndex()const;
            [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle()const;
        private:
            UINT mDescriptorIndex{}; // ディスクリプタの生成順
        protected:
            DescriptorHandle* mpHandle{};
            DescriptorPool* mpPool{};
            OrcaComPtr(ID3D12Resource) mpResource {};
        };
    }
}

