#include"pch.h"


#include"GraphicsForGameLoop.h"
#include"GraphicsImpl.h"

void Graphics::GraphicsForGameLoop::Initialize(HWND Hwnd_)
{
    mpImpl = std::make_unique<GraphicsImpl>();
    mpImpl->Initialize(Hwnd_);
}

void Graphics::GraphicsForGameLoop::Finalize()
{
    mpImpl->Finalize();
}

void Graphics::GraphicsForGameLoop::OpenCmdList()
{
    mpImpl->OpenCmdList();
}

void Graphics::GraphicsForGameLoop::CloseCmdList()
{
    mpImpl->CloseCmdList();
}

void Graphics::GraphicsForGameLoop::StackCmdList()
{
    mpImpl->StackCmdList();
}
