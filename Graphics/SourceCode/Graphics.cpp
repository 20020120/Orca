#include"pch.h"
//#include<memory>

#include"Graphics.h"
#include"GraphicsImpl.h"

// Staticƒƒ“ƒo‚Ì‰Šú‰»
std::unique_ptr<OrcaGraphics::GraphicsImpl> OrcaGraphics::Graphics::mpImpl{};

Microsoft::WRL::ComPtr<ID3D12Device> OrcaGraphics::Graphics::GetDevice()
{
    return mpImpl->GetDevice();
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> OrcaGraphics::Graphics::GetCmdList()
{
    return mpImpl->GetCmdList();
}

OrcaGraphics::DescriptorPool* OrcaGraphics::Graphics::GetDescriptorPool(POOL_TYPE Type_)
{
    return mpImpl->GetDescriptorPool(Type_);
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> OrcaGraphics::Graphics::GetCommandQueue()
{
    return mpImpl->GetCommandQueue();
}
