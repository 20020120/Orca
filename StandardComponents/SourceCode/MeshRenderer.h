#pragma once
#include"Renderer.h"
#include"ShaderType.h"
#include<wrl.h>

// ------------------------------------ �O���錾 -----------------------------------
struct ID3D12GraphicsCommandList;

namespace System
{
    class RenderSystem;
}


// -------------------------------- �`��N���X�̊��N���X --------------------------------
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
