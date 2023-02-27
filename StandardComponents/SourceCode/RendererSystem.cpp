#include"RendererSystem.h"
#include "ObjRenderer.h"
#include"ShaderHolder.h"
#include"MacroMemory.h"
#include<d3d12.h>

void System::RenderSystem::OnAwake()
{
    ORCA_MAKE_UNIQUE(mpShaderHolder);
}

void System::RenderSystem::Finalize()
{
    mpShaderHolder.reset();
}

void System::RenderSystem::Update(float Dt_)
{
    BaseUpdate(Dt_);
}

void System::RenderSystem::Begin(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& pCmdList_) const
{
    mpShaderHolder->Set(Graphics::Shader::ShaderType::Sample, pCmdList_.Get());
}

void System::RenderSystem::Render(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& pCmdList_) const
{
    for(const auto& p: mComponents)
    {
        mpShaderHolder->Set(p->mShaderType, pCmdList_.Get());
        p->StackGraphicsCmd(pCmdList_);
    }
}
