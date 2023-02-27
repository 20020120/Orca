#include"pch.h"
//#include<memory>

#include"Graphics.h"
#include"GraphicsImpl.h"

// Staticƒƒ“ƒo‚Ì‰Šú‰»
std::unique_ptr<Graphics::GraphicsImpl> Graphics::Graphics::mpImpl{};

Microsoft::WRL::ComPtr<ID3D12Device> Graphics::Graphics::GetDevice()
{
    return mpImpl->GetDevice();
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Graphics::Graphics::GetCmdList()
{
    return mpImpl->GetCmdList();
}

Graphics::DescriptorPool* Graphics::Graphics::GetDescriptorPool(POOL_TYPE Type_)
{
    return mpImpl->GetDescriptorPool(Type_);
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> Graphics::Graphics::GetCommandQueue()
{
    return mpImpl->GetCommandQueue();
}
