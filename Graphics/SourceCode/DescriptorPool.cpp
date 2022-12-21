#include "pch.h"
#include"DescriptorPool.h"

bool OrcaGraphics::DescriptorPool::Create(ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC* pDesc,
    DescriptorPool** ppPool)
{
    // �����`�F�b�N.
    if (pDevice == nullptr || pDesc == nullptr || ppPool == nullptr)
    {
        return false;
    }

    // �C���X�^���X�𐶐����܂�.
    auto instance = new (std::nothrow) DescriptorPool();
    if (instance == nullptr)
    {
        return false;
    }

    // �f�B�X�N���v�^�q�[�v�𐶐�.
    auto hr = pDevice->CreateDescriptorHeap(
        pDesc,
        IID_PPV_ARGS(instance->m_pHeap.GetAddressOf()));

    // ���s���������������s���ďI�����܂�.
    if (FAILED(hr))
    {
        instance->Release();
        return false;
    }

    // �v�[�������������܂�.
    if (!instance->m_Pool.Initialize(pDesc->NumDescriptors))
    {
        instance->Release();
        return false;
    }

    // �f�B�X�N���v�^�̉��Z�T�C�Y���擾���܂�.
    instance->m_DescriptorSize =
        pDevice->GetDescriptorHandleIncrementSize(pDesc->Type);

    // �C���X�^���X��ݒ�.
    *ppPool = instance;

    // ����I��.
    return true;
}

void OrcaGraphics::DescriptorPool::AddRef()
{
    ++m_RefCount;
}

void OrcaGraphics::DescriptorPool::Release()
{
    --m_RefCount;
    if (m_RefCount == 0)
    {
        delete this;
    }
}

uint32_t OrcaGraphics::DescriptorPool::GetCount() const
{
    return m_RefCount;
}

OrcaGraphics::DescriptorHandle* OrcaGraphics::DescriptorPool::AllocHandle()
{
    // �������֐��ł�.
    auto func = [&](uint32_t index, DescriptorHandle* pHandle)
    {
        auto handleCPU = m_pHeap->GetCPUDescriptorHandleForHeapStart();
        handleCPU.ptr += m_DescriptorSize * static_cast<unsigned long long>(index);

        auto handleGPU = m_pHeap->GetGPUDescriptorHandleForHeapStart();
        handleGPU.ptr += m_DescriptorSize * static_cast<unsigned long long>(index);

        pHandle->HandleCPU = handleCPU;
        pHandle->HandleGPU = handleGPU;
    };

    // �������֐������s���Ă���n���h����ԋp���܂�.
    return m_Pool.Alloc(func);
}

void OrcaGraphics::DescriptorPool::FreeHandle(DescriptorHandle*& pHandle)
{
    if (pHandle != nullptr)
    {
        // �n���h�����v�[���ɖ߂��܂�.
        m_Pool.Free(pHandle);
        // nullptr�ŃN���A���Ă����܂�.
        pHandle = nullptr;
    }
}

uint32_t OrcaGraphics::DescriptorPool::GetAvailableHandleCount() const
{
    return m_Pool.GetAvailableCount();
}

uint32_t OrcaGraphics::DescriptorPool::GetAllocatedHandleCount() const
{
    return m_Pool.GetUsedCount();
}

uint32_t OrcaGraphics::DescriptorPool::GetHandleCount() const
{
    return m_Pool.GetSize();
}

ID3D12DescriptorHeap* const OrcaGraphics::DescriptorPool::GetHeap() const
{
    return m_pHeap.Get();
}

OrcaGraphics::DescriptorPool::DescriptorPool()
    : m_RefCount(1)
    , m_Pool()
    , m_pHeap()
    , m_DescriptorSize(0)
{}

OrcaGraphics::DescriptorPool::~DescriptorPool()
{
    m_Pool.Finalize();
    m_pHeap.Reset();
    m_DescriptorSize = 0;
}
