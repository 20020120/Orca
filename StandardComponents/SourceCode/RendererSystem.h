#pragma once
#include"SystemBase.h"
#include"ShaderHolder.h"
#include<wrl.h>
#include<memory>
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
        void OnAwake() override;
        void Finalize();
        void Update(float Dt_);
        void Begin(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& pCmdList_) const;
        void Render(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& pCmdList_) const;
    private:
        std::unique_ptr<Graphics::ShaderHolder> mpShaderHolder{};
    };
}
