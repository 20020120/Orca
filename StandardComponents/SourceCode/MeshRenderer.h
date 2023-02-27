#pragma once
#include"Renderer.h"
#include"ShaderType.h"
#include<wrl.h>

// ------------------------------------ 前方宣言 -----------------------------------
struct ID3D12GraphicsCommandList;

namespace System
{
    class RenderSystem;
}


// -------------------------------- 描画クラスの基底クラス --------------------------------
namespace Component
{
    class MeshRenderer final :public Renderer
    {
        friend class System::RenderSystem;
    public:
        explicit MeshRenderer(const Graphics::Shader::ShaderType& ShaderType_);
        ~MeshRenderer() override = default;
        void OnStart() override;
    private:
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)override;
    };
}
