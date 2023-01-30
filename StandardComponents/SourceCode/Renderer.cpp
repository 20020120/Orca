#include"Renderer.h"
#include"RendererSystem.h"
Component::Renderer::Renderer()
    :Component(0)
{
    System::RenderSystem::Instance().Add(shared_from_this());
}


