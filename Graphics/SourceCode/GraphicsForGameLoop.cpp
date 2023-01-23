#include"pch.h"


#include"GraphicsForGameLoop.h"
#include"GraphicsImpl.h"

void OrcaGraphics::GraphicsForGameLoop::Initialize(HWND Hwnd_)
{
    mpImpl = std::make_unique<GraphicsImpl>();
    mpImpl->Initialize(Hwnd_);
}

void OrcaGraphics::GraphicsForGameLoop::Finalize()
{
    mpImpl->Finalize();
}

void OrcaGraphics::GraphicsForGameLoop::OpenCmdList()
{
    mpImpl->OpenCmdList();
}

void OrcaGraphics::GraphicsForGameLoop::CloseCmdList()
{
    mpImpl->CloseCmdList();
}

void OrcaGraphics::GraphicsForGameLoop::StackCmdList()
{
    mpImpl->StackCmdList();
}
