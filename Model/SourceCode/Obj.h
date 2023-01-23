#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include <memory>
#include<wrl.h>
#include<vector>

#include"Graphics.h"
#include"Texture.h"
#include"ConstantBuffer.h"
#include"GraphicsMacro.h"

// 前方宣言
namespace OrcaGraphics
{
    class DescriptorPool;
}

namespace Model
{
    // Objファイルのクラス
    class Obj
    {
        // 前方宣言
        struct VertexData;
        struct Cb_Obj;

    public:
        Obj() = default;
        ~Obj();

        // 初期化
        void Initialize(const wchar_t* ObjPath_);
        void Update(float Dt_); // 更新
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_); // 描画コマンドを積む
    private:
        static void Parse(const wchar_t* ObjPath_, std::vector<VertexData>& Vertices_, std::vector<uint32_t>& Indices_,
            std::wstring& TextureName_);
        void CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<VertexData>& Vertices_);
        void CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<uint32_t>& Indices_);
        void CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, OrcaGraphics::DescriptorPool* pPool_);
        void CreateTexture(OrcaComPtr(ID3D12Device) pDevice_, OrcaGraphics::DescriptorPool* pPool_,
            OrcaComPtr(ID3D12CommandQueue) pCommandQueue_, std::wstring TexturePath_);

        void CreateDx12Resource();
    private:
        // ------------------------------- 頂点読み込み ------------------------------
        struct VertexData
        {
            DirectX::XMFLOAT3 mPosition{};
            DirectX::XMFLOAT3 mNormal{};
            DirectX::XMFLOAT2 mUv{};
        };
        Microsoft::WRL::ComPtr<ID3D12Resource> mpVertexBuffer{};
        D3D12_VERTEX_BUFFER_VIEW mVbView{};
        UINT m_VertexCounts{};  // 頂点数
        // ----------------------------- インデックスバッファ ----------------------------
        Microsoft::WRL::ComPtr<ID3D12Resource> mpIndexBuffer{};
        D3D12_INDEX_BUFFER_VIEW mIbView{};

        // ------------------------------- 定数バッファ ------------------------------
        struct alignas(256)Cb_Obj
        {
            DirectX::XMFLOAT4X4 World{};
        };
        Cb_Obj* mCbData{};
        std::unique_ptr<OrcaGraphics::Resource::ConstantBuffer> mCb{};

       // -------------------------------- テクスチャ -------------------------------
       OrcaGraphics::Texture mTexture{};
    };
}
