#include "pch.h"
#include "Dx12IndexBuffer.h"
#include"GraphicsLogger.h"
bool OrcaGraphics::Resource::IndexBuffer::Create(const std::vector<uint32_t>& Indices_)
{
    const auto pDevice = Graphics::GetDevice();

    // ------------------------------ ヒーププロパティを設定 ------------------------------
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_UPLOAD;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // リソースの設定
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeof(uint32_t) * Indices_.size();
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // リソースを生成
    auto hr = pDevice->CreateCommittedResource(
        &prp,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpBuffer.ReleaseAndGetAddressOf())
    );
    OrcaDebug::GraphicsLog("インデックスバッファを作成", hr);

    // マッピング
    void* ptr = nullptr;
    hr = mpBuffer->Map(0, nullptr, &ptr);
    OrcaDebug::GraphicsLog("マッピング", hr);
    memcpy(ptr, Indices_.data(), sizeof(uint32_t) * Indices_.size());
    mpBuffer->Unmap(0, nullptr);

    // インデックスバッファビューを作成
    mIbView.BufferLocation = mpBuffer->GetGPUVirtualAddress();
    mIbView.Format = DXGI_FORMAT_R32_UINT;
    mIbView.SizeInBytes = static_cast<UINT>(sizeof(uint32_t) * Indices_.size());
    return true;
}
