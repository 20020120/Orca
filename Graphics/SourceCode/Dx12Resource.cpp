#include "pch.h"
#include "Dx12Resource.h"
#include "DescriptorHandle.h"
#include "OrcaException.h"
#include"DescriptorPool.h"

OrcaGraphics::Resource::Dx12Resource::Dx12Resource(DescriptorPool* pDescriptorPool_)
{
    Orca_NotNullException(mpPool);
    Orca_NotNullException(mpHandle);

    mpPool = pDescriptorPool_;
    mpPool->AddRef();
    mDescriptorIndex = mpPool->GetCount();
    mpHandle = mpPool->AllocHandle();
    Orca_NullException(mpHandle);
}

OrcaGraphics::Resource::Dx12Resource::~Dx12Resource()
{
    // メモリマッピングを解除して，定数バッファを解放します.
    if (mpResource != nullptr)
    {
        mpResource->Unmap(0, nullptr);
        mpResource.Reset();
    }
    // ビューを破棄.
    if (mpPool != nullptr)
    {
        mpPool->FreeHandle(mpHandle);
        mpHandle = nullptr;
    }
    // ディスクリプタプールを解放.
    if (mpPool != nullptr)
    {
        mpPool->Release();
        mpPool = nullptr;
    }
}

uint32_t OrcaGraphics::Resource::Dx12Resource::GetDescriptorIndex() const
{
    return mDescriptorIndex;
}

D3D12_GPU_DESCRIPTOR_HANDLE OrcaGraphics::Resource::Dx12Resource::GetGpuHandle() const
{
    return mpHandle->HandleGPU;
}
