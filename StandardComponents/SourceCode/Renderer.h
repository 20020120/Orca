#pragma once
#include"Component.h"
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
    class Renderer :public std::enable_shared_from_this<Renderer>, public Component
    {
        friend class System::RenderSystem;
    public:
        explicit Renderer(const OrcaGraphics::Shader::ShaderType& ShaderType_);
        ~Renderer() override = default;
        void Start() override;
    private:
        virtual void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) = 0;
    protected:
        OrcaGraphics::Shader::ShaderType mShaderType;
    };
}
