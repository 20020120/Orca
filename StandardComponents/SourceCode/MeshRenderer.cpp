#include"MeshRenderer.h"
#include"Dx12ResourceHolder.h"

#include<d3d12.h>

#include "GameObject.h"

Component::MeshRenderer::MeshRenderer(const OrcaGraphics::Shader::ShaderType& ShaderType_)
    :Renderer(ShaderType_)
{
}

void Component::MeshRenderer::OnStart()
{
    Renderer::OnStart();
    // ------------------------- シェーダーの種類に応じたリソース情報を生成 -------------------------
    OrcaGraphics::Dx12ResourceHolder::Add(mShaderType, mpGameObject.lock()->GetName());
}

void Component::MeshRenderer::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{

}
