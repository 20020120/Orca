#pragma once
#include <d3d12.h>
#include <intsafe.h>
#include"GraphicsMacro.h"

namespace OrcaGraphics
{
    // ---------------------------------- 前方宣言 ---------------------------------
    class DescriptorPool;
    class DescriptorHandle;

    // ---------------------------------- 描画用のリソースの基底クラス ---------------------------------
    class RenderResource
    {
    public:
        // 共通の初期化
        RenderResource(DescriptorPool* pDescriptorPool_, UINT RootParamIndex);
        virtual ~RenderResource();
        void Bind(OrcaComPtr(ID3D12GraphicsCommandList) pCmdList_) const; // GPUにバインド

    private:
        UINT mRootParamIndex{}; // ルートパラメーターの番号
    protected:
        DescriptorHandle* mpHandle{};
        DescriptorPool* mpPool{};
        OrcaComPtr(ID3D12Resource) mpResource {};
    };
}

