#pragma once
#include"SystemBase.h"
#include<wrl.h>
namespace Component
{
    class Renderer;
}

struct ID3D12GraphicsCommandList;

namespace System
{
    class RenderSystem final :public Base::SystemBase<Component::Renderer, RenderSystem>
    {
        // ----------------------------- 公開先クラスを指定 -----------------------------
        friend class Component::Renderer;
    public:
        void Update(float Dt_);
        void Render(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& pCmdList_) const;
    };
}
