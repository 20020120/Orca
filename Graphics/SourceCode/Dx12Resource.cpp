#include "pch.h"
#include "Dx12Resource.h"
#include "DescriptorHandle.h"
#include "OrcaException.h"
#include"DescriptorPool.h"

Graphics::Resource::Dx12Resource::Dx12Resource(DescriptorPool* pDescriptorPool_)
{
    Orca_NotNullException(mpPool);
    Orca_NotNullException(mpHandle);

    mpPool = pDescriptorPool_;
    mpPool->AddRef();
    mpHandle = mpPool->AllocHandle();
    Orca_NullException(mpHandle);
}

Graphics::Resource::Dx12Resource::~Dx12Resource()
{
    // �������}�b�s���O���������āC�萔�o�b�t�@��������܂�.
    if (mpResource != nullptr)
    {
        mpResource->Unmap(0, nullptr);
        mpResource.Reset();
    }
    // �r���[��j��.
    if (mpPool != nullptr)
    {
        mpPool->FreeHandle(mpHandle);
        mpHandle = nullptr;
    }
    // �f�B�X�N���v�^�v�[�������.
    if (mpPool != nullptr)
    {
        mpPool->Release();
        mpPool = nullptr;
    }
}

uint32_t Graphics::Resource::Dx12Resource::GetDescriptorIndex() const
{
    return mpHandle->DescriptorIndex;
}

D3D12_GPU_DESCRIPTOR_HANDLE Graphics::Resource::Dx12Resource::GetGpuHandle() const
{
    return mpHandle->HandleGPU;
}
