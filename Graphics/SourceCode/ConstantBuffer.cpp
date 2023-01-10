#include"pch.h"
#include"ConstantBuffer.h"

#include "DescriptorPool.h"
#include"OrcaException.h"
#include"GraphicsLogger.h"
OrcaGraphics::ConstantBuffer::~ConstantBuffer()
{
    Finalize();
}

bool OrcaGraphics::ConstantBuffer::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_,
    size_t Size_)
{
    Orca_NullException(pDevice_);
    Orca_NullException(pPool_);
    Orca_LessZeroException(Size_);

    Orca_NotNullException(mpPool);
    Orca_NotNullException(mpHandle);

    mpPool = pPool_;
    mpPool->AddRef();
    
    constexpr size_t align = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
    const UINT64 sizeAligned = (Size_ + (align - 1)) & ~(align - 1); // align�ɐ؂�グ��.

    // �q�[�v�v���p�e�B.
    D3D12_HEAP_PROPERTIES prop;
    prop.Type = D3D12_HEAP_TYPE_UPLOAD;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 1;
    prop.VisibleNodeMask = 1;

    // ���\�[�X�̐ݒ�.
    D3D12_RESOURCE_DESC desc;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeAligned;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // ���\�[�X�𐶐�.
    auto hr = pDevice_->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpCb.GetAddressOf()));
    OrcaDebug::GraphicsLog("�萔�o�b�t�@�F���\�[�X�𐶐�", hr);

    // �������}�b�s���O���Ă����܂�.
    hr = mpCb->Map(0, nullptr, &mpMappedPtr);
    OrcaDebug::GraphicsLog("�萔�o�b�t�@�F�������}�b�s���O", hr);

    mDesc.BufferLocation = mpCb->GetGPUVirtualAddress();
    mDesc.SizeInBytes = UINT(sizeAligned);
    mpHandle = mpPool->AllocHandle();

    pDevice_->CreateConstantBufferView(&mDesc, mpHandle->HandleCPU);

    // ����I��.
    return true;
}

void OrcaGraphics::ConstantBuffer::Finalize()
{
    // �������}�b�s���O���������āC�萔�o�b�t�@��������܂�.
    if (mpCb != nullptr)
    {
        mpCb->Unmap(0, nullptr);
        mpCb.Reset();
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

    mpMappedPtr = nullptr;
}

D3D12_GPU_VIRTUAL_ADDRESS OrcaGraphics::ConstantBuffer::GetAddress() const
{
    return mDesc.BufferLocation;
}

D3D12_CPU_DESCRIPTOR_HANDLE OrcaGraphics::ConstantBuffer::GetCPU() const
{
    if (mpHandle == nullptr)
    {
        return D3D12_CPU_DESCRIPTOR_HANDLE();
    }

    return mpHandle->HandleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE OrcaGraphics::ConstantBuffer::GetGPU() const
{
    if (mpHandle == nullptr)
    {
        return D3D12_GPU_DESCRIPTOR_HANDLE();
    }

    return mpHandle->HandleGPU;
}

void* OrcaGraphics::ConstantBuffer::GetPtr() const
{
    return mpMappedPtr;
}

