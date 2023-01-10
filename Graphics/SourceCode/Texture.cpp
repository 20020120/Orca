#include "pch.h"
#include<DDSTextureLoader.h>
#include<ResourceUploadBatch.h>

#include"Texture.h"
#include "DescriptorPool.h"
#include"OrcaException.h"
#include"GraphicsLogger.h"

OrcaGraphics::Texture::Texture()
{}


bool OrcaGraphics::Texture::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_,
    const D3D12_RESOURCE_DESC* pDesc_, bool IsCube_)
{
    // 引数チェック
    Orca_NullException(pDevice_);
    Orca_NullException(pPool_);
    Orca_NullException(pDesc_);

    // ディスクリプタプールを設定
    mpPool = pPool_;
    mpPool->AddRef();

    mpHandle = pPool_->AllocHandle();
    Orca_NullException(mpHandle);

    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_DEFAULT;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.CreationNodeMask = 0;
    prop.VisibleNodeMask = 0;

    const auto hr = pDevice_->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        pDesc_,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(mpResource.GetAddressOf())
    );
    OrcaDebug::GraphicsLog("コミットリソースを作成", hr);

    // シェーダリソースビューの設定を求める.
    const auto viewDesc = GetViewDesc(IsCube_);
    // シェーダリソースビューを生成します.
    pDevice_->CreateShaderResourceView(mpResource.Get(), &viewDesc, mpHandle->HandleCPU);

    return true;
}

bool OrcaGraphics::Texture::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_,
    const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_)
{
    // 引数チェック
    Orca_NullException(pDevice_);
    Orca_NullException(pPool_);

    // ディスクリプタプールを設定
    mpPool = pPool_;
    mpPool->AddRef();

    mpHandle = pPool_->AllocHandle();
    Orca_NullException(mpHandle);

    // ファイルからテクスチャを生成
    bool isCube = false;
    const auto hr = DirectX::CreateDDSTextureFromFile(
        pDevice_.Get(),
        Batch_,
        FileName_,
        mpResource.GetAddressOf(),
        true,
        0,
        nullptr,
        &isCube
    );
    OrcaDebug::GraphicsLog("テクスチャを生成", hr);

    // シェーダーリソースビューの設定を求める
    const auto viewDesc = GetViewDesc(isCube);
    // シェーダーリソースビューを生成する
    pDevice_->CreateShaderResourceView(mpResource.Get(), &viewDesc, mpHandle->HandleCPU);
    return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE OrcaGraphics::Texture::GetHandleCPU() const
{
    return mpHandle->HandleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE OrcaGraphics::Texture::GetHandleGPU() const
{
    return mpHandle->HandleGPU;
}

D3D12_SHADER_RESOURCE_VIEW_DESC OrcaGraphics::Texture::GetViewDesc(bool IsCube_) const
{
    auto desc = mpResource->GetDesc();
    D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};

    viewDesc.Format = desc.Format;
    viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    switch (desc.Dimension)
    {
    case D3D12_RESOURCE_DIMENSION_BUFFER:
    {
        // バッファは対象外とします.
        abort();    // アプリを止める.
    }
    break;

    case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
    {
        if (desc.DepthOrArraySize > 1)
        {
            viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;

            viewDesc.Texture1DArray.MostDetailedMip = 0;
            viewDesc.Texture1DArray.MipLevels = desc.MipLevels;
            viewDesc.Texture1DArray.FirstArraySlice = 0;
            viewDesc.Texture1DArray.ArraySize = desc.DepthOrArraySize;
            viewDesc.Texture1DArray.ResourceMinLODClamp = 0.0f;
        }
        else
        {
            viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;

            viewDesc.Texture1D.MostDetailedMip = 0;
            viewDesc.Texture1D.MipLevels = desc.MipLevels;
            viewDesc.Texture1D.ResourceMinLODClamp = 0.0f;
        }
    }
    break;

    case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
    {
        if (IsCube_)
        {
            if (desc.DepthOrArraySize > 6)
            {
                viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;

                viewDesc.TextureCubeArray.MostDetailedMip = 0;
                viewDesc.TextureCubeArray.MipLevels = desc.MipLevels;
                viewDesc.TextureCubeArray.First2DArrayFace = 0;
                viewDesc.TextureCubeArray.NumCubes = (desc.DepthOrArraySize / 6);
                viewDesc.TextureCubeArray.ResourceMinLODClamp = 0.0f;
            }
            else
            {
                viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;

                viewDesc.TextureCube.MostDetailedMip = 0;
                viewDesc.TextureCube.MipLevels = desc.MipLevels;
                viewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
            }
        }
        else
        {
            if (desc.DepthOrArraySize > 1)
            {
                if (desc.MipLevels > 1)
                {
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;

                    viewDesc.Texture2DMSArray.FirstArraySlice = 0;
                    viewDesc.Texture2DMSArray.ArraySize = desc.DepthOrArraySize;
                }
                else
                {
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;

                    viewDesc.Texture2DArray.MostDetailedMip = 0;
                    viewDesc.Texture2DArray.MipLevels = desc.MipLevels;
                    viewDesc.Texture2DArray.FirstArraySlice = 0;
                    viewDesc.Texture2DArray.ArraySize = desc.DepthOrArraySize;
                    viewDesc.Texture2DArray.PlaneSlice = 0;
                    viewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
                }
            }
            else
            {
                if (desc.MipLevels > 1)
                {
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
                }
                else
                {
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

                    viewDesc.Texture2D.MostDetailedMip = 0;
                    viewDesc.Texture2D.MipLevels = desc.MipLevels;
                    viewDesc.Texture2D.PlaneSlice = 0;
                    viewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
                }
            }
        }
    }
    break;

    case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
    {
        viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;

        viewDesc.Texture3D.MostDetailedMip = 0;
        viewDesc.Texture3D.MipLevels = desc.MipLevels;
        viewDesc.Texture3D.ResourceMinLODClamp = 0.0f;
    }
    break;

    default:
    {
        // 想定外
        abort();    // アプリを止める.
    }
    break;
    }

    return viewDesc;
}
