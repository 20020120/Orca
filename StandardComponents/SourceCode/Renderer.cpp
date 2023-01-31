#include"Renderer.h"
#include"RendererSystem.h"
Component::Renderer::Renderer()
    :Component(0)
{}

void Component::Renderer::Start()
{
    System::RenderSystem::Instance().Add(shared_from_this());
}


