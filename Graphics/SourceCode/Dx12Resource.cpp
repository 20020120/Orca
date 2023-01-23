#include "pch.h"
#include "Dx12Resource.h"
#include "DescriptorHandle.h"
#include "OrcaException.h"
#include"DescriptorPool.h"

OrcaGraphics::Resource::Dx12Resource::Dx12Resource(DescriptorPool * pDescriptorPool_, UINT RootParamIndex)
    :mRootParamIndex(RootParamIndex)
{
    Orca_NotNullException(mpPool);
    Orca_NotNullException(mpHandle);

    mpPool = pDescriptorPool_;
    mpPool->AddRef();
}

OrcaGraphics::Resource::Dx12Resource::~Dx12Resource()
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

void OrcaGraphics::Resource::Dx12Resource::Bind(const OrcaComPtr(ID3D12GraphicsCommandList) pCmdList_) const
{
    pCmdList_->SetGraphicsRootDescriptorTable(mRootParamIndex, mpHandle->HandleGPU);
}

std::string OrcaGraphics::Resource::Dx12Resource::GetName() const
{
    return mName;
}
