#pragma once
#include"Vector3.h"

#include"Texture.h"
#include"ConstantBuffer.h"

#include<wrl.h>
#include<d3d12.h>
#include<memory>

// ------------------------------------ �O���錾 -----------------------------------
namespace OrcaGraphics
{
    class DescriptorPool;
}


namespace Model
{
    struct ObjResource
    {
        struct VertexData;

        static void Parse(const wchar_t* ObjPath_, std::vector<VertexData>& Vertices_, std::vector<uint32_t>& Indices_,
            std::wstring& TextureName_);
        void CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<VertexData>& Vertices_);
        void CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<uint32_t>& Indices_);
        void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, OrcaGraphics::DescriptorPool* pPool_);
        void CreateTexture(OrcaComPtr(ID3D12Device) pDevice_, OrcaGraphics::DescriptorPool* pPool_,
            OrcaComPtr(ID3D12CommandQueue) pCommandQueue_, std::wstring TexturePath_);


        // ------------------------------- ���_�ǂݍ��� ------------------------------
        struct VertexData
        {
            Math::Vector3 mPosition{};
            Math::Vector3 mNormal{};
            DirectX::XMFLOAT2 mUv{};
        };
        Microsoft::WRL::ComPtr<ID3D12Resource> mpVertexBuffer{};
        D3D12_VERTEX_BUFFER_VIEW mVbView{};
        UINT m_VertexCounts{};  // ���_��
        // ----------------------------- �C���f�b�N�X�o�b�t�@ ----------------------------
        Microsoft::WRL::ComPtr<ID3D12Resource> mpIndexBuffer{};
        D3D12_INDEX_BUFFER_VIEW mIbView{};

        // ------------------------------- �萔�o�b�t�@ ------------------------------
        struct alignas(256)Cb_Obj
        {
            DirectX::XMFLOAT4X4 World{};
        };
        Cb_Obj* mCbData{};
        std::unique_ptr<OrcaGraphics::Resource::ConstantBuffer> mCb{};

        // -------------------------------- �e�N�X�`�� -------------------------------
        OrcaGraphics::Texture mTexture{};
    };
}
