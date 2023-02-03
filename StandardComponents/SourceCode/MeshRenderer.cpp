#include"MeshRenderer.h"


#include<d3d12.h>
Component::MeshRenderer::MeshRenderer(const OrcaGraphics::Shader::ShaderType& ShaderType_)
    :Renderer(ShaderType_)
{
}

void Component::MeshRenderer::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{

}
