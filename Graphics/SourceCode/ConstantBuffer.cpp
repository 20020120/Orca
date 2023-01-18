#include"pch.h"
#include"ConstantBuffer.h"

#include "DescriptorPool.h"
#include"OrcaException.h"
#include"GraphicsLogger.h"
#include"GraphicsMacro.h"
OrcaGraphics::ConstantBuffer::ConstantBuffer(OrcaComPtr(ID3D12Device) pDevice_, DescriptorPool* pPool_,
    size_t Size_, UINT RootParamIndex_, void** MappedPtr_)
    :RenderResource(pPool_, RootParamIndex_)
{
    Orca_NullException(pDevice_);
    Orca_NullException(pPool_);
    Orca_LessZeroException(Size_);


    constexpr size_t align = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
    const UINT64 sizeAligned = (Size_ + (align - 1)) & ~(align - 1); // alignに切り上げる.

    // ヒーププロパティ.
    D3D12_HEAP_PROPERTIES prop;
    prop.Type = D3D12_HEAP_TYPE_UPLOAD;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 1;
    prop.VisibleNodeMask = 1;

    // リソースの設定.
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

    // リソースを生成.
    auto hr = pDevice_->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpResource.GetAddressOf()));
    OrcaDebug::GraphicsLog("定数バッファ：リソースを生成", hr);

    // メモリマッピングしておきます.
    hr = mpResource->Map(0, nullptr, MappedPtr_);
    OrcaDebug::GraphicsLog("定数バッファ：メモリマッピング", hr);

    mDesc.BufferLocation = mpResource->GetGPUVirtualAddress();
    mDesc.SizeInBytes = static_cast<UINT>(sizeAligned);
    // ハンドルを初期化
    mpHandle = mpPool->AllocHandle();
    pDevice_->CreateConstantBufferView(&mDesc, mpHandle->HandleCPU);
}
