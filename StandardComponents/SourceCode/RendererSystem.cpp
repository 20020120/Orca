#include"RendererSystem.h"
#include "ObjRenderer.h"

#include<d3d12.h>
void System::RenderSystem::Update(float Dt_)
{
    BaseUpdate(Dt_);
}

void System::RenderSystem::Render(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& pCmdList_) const
{
    for(const auto& p: mComponents)
    {
        p->StackGraphicsCmd(pCmdList_);
    }
}
