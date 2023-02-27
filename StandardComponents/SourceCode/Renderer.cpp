#include"Renderer.h"
#include"RendererSystem.h"
Component::Renderer::Renderer(const Graphics::Shader::ShaderType& ShaderType_)
    :Component(0)
    , mShaderType(ShaderType_)
{}

void Component::Renderer::OnStart()
{
    System::RenderSystem::Instance().Add(shared_from_this());
}


