#pragma once
#include"Graphics.h"
#include"GraphicsLogger.h"
#include<vector>
#include<d3d12.h>
#include<wrl.h>

// ----------------------------------- ���_�o�b�t�@ ----------------------------------
namespace Graphics
{
    namespace Resource
    {
        class VertexBuffer
        {
        public:
            // --------------------------- ���_�o�b�t�@���쐬 ---------------------------
            template<class Vertex>
            bool Create(const std::vector<Vertex>& Vertices_);  

            Microsoft::WRL::ComPtr<ID3D12Resource> mpBuffer{};
            D3D12_VERTEX_BUFFER_VIEW mVbView{};
        };
    }
}

template <class Vertex>
bool Graphics::Resource::VertexBuffer::Create(const std::vector<Vertex>& Vertices_)
{
    const auto pDevice = Graphics::GetDevice();

    // ------------------------------ ���_�o�b�t�@���쐬���� ------------------------------
    // �q�[�v�v���p�e�B�̐ݒ�
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_UPLOAD;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // ���\�[�X�̐ݒ�
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeof(Vertex) * Vertices_.size();
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    auto hr = pDevice->CreateCommittedResource(
        &prp,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpBuffer.GetAddressOf())
    );

    OrcaDebug::GraphicsLog("���_�o�b�t�@���쐬", hr);

    // �}�b�s���O����
    void* ptr{};
    hr = mpBuffer->Map(0, nullptr, &ptr);
    OrcaDebug::GraphicsLog("�}�b�s���O", hr);

    // ���_�f�[�^���}�b�s���O��Ɏw��
    memcpy(ptr, Vertices_.data(), sizeof(Vertex) * Vertices_.size());

    // �}�b�s���O����
    mpBuffer->Unmap(0, nullptr);

    // �r���[�̐ݒ�
    mVbView.BufferLocation = mpBuffer->GetGPUVirtualAddress();
    mVbView.SizeInBytes = sizeof(Vertex) * static_cast<UINT>(Vertices_.size());
    mVbView.StrideInBytes = sizeof(Vertex);


    return true;
}