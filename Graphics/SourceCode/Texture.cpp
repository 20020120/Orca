#include "pch.h"
#include<DDSTextureLoader.h>
#include<WICTextureLoader.h>
#include<ResourceUploadBatch.h>
#include<filesystem>

#include"Texture.h"
#include "DescriptorPool.h"
#include"OrcaException.h"
#include"GraphicsLogger.h"

OrcaGraphics::Resource::Texture::Texture(DirectX::ResourceUploadBatch& Batch_, const wchar_t* FileName_)
    :Dx12Resource(Graphics::GetDescriptorPool(POOL_TYPE_RES))
{
    Load(FileName_, Batch_);
}


bool OrcaGraphics::Resource::Texture::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_,
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

void OrcaGraphics::Resource::Texture::Load(const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_)
{
    // テクスチャをロードする
    LoadTextureFromFile(Graphics::GetDevice(), FileName_, Batch_);
}

void OrcaGraphics::Resource::Texture::LoadTextureFromFile(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const wchar_t* FileName_,
    DirectX::ResourceUploadBatch& Batch_)
{
    // ----------------------------- テクスチャの種類を判定する -----------------------------
    const std::filesystem::path fileName = FileName_;
    if (!exists(fileName))
    {
        throw std::logic_error("ファイルを開くのに失敗しました");
    }
    // 拡張子を取得
    const auto extension = fileName.extension().string();
    // ----------------------------- DDS形式かどうかを判定する ----------------------------
    if (std::strcmp(extension.c_str(), ".dds") == 0)
    {
        DDSLoadTexture(pDevice_, FileName_, Batch_);
        return;
    }
    // ------------------------------- DDS形式以外の形式 ------------------------------
    WICLoadTexture(pDevice_, FileName_, Batch_);

}

void OrcaGraphics::Resource::Texture::DDSLoadTexture(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const wchar_t* FileName_,
    DirectX::ResourceUploadBatch& Batch_)
{
    // ---------------------------- DDSファイルをロードする関数 ----------------------------
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
    OrcaDebug::GraphicsLog("DDSファイルからテクスチャを生成", hr);

    const auto viewDesc = GetViewDesc(isCube);
    pDevice_->CreateShaderResourceView(mpResource.Get(), &viewDesc, mpHandle->HandleCPU);
}

void OrcaGraphics::Resource::Texture::WICLoadTexture(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const wchar_t* FileName_,
    DirectX::ResourceUploadBatch& Batch_)
{
    // ---------------------------- DDSファイル以外をロードする関数 ----------------------------
    // ファイルからテクスチャを生成
    const auto hr = DirectX::CreateWICTextureFromFile(
        pDevice_.Get(),
        Batch_,
        FileName_,
        mpResource.GetAddressOf(),
        true
    );
    OrcaDebug::GraphicsLog("DDS以外からテクスチャを生成", hr);

    const auto viewDesc = mpResource->GetDesc();
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = viewDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = viewDesc.MipLevels;
    pDevice_->CreateShaderResourceView(mpResource.Get(), &srvDesc, mpHandle->HandleCPU);
}

D3D12_SHADER_RESOURCE_VIEW_DESC OrcaGraphics::Resource::Texture::GetViewDesc(bool IsCube_) const
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

