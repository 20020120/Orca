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
namespace Graphics
{
    class DescriptorPool;
}

namespace Resource
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
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) const; // 描画コマンドを積む
        std::wstring GetTextureName()const;
    private:
        static void Parse(const wchar_t* ObjPath_, std::vector<VertexData>& Vertices_, std::vector<uint32_t>& Indices_,
            std::wstring& TextureName_);
        void CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<VertexData>& Vertices_);
        void CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const std::vector<uint32_t>& Indices_);
        void CreateTexture(OrcaComPtr(ID3D12Device) pDevice_, Graphics::DescriptorPool* pPool_,
            OrcaComPtr(ID3D12CommandQueue) pCommandQueue_, std::wstring TexturePath_);
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
        // ------------------------------ テクスチャの名前 -----------------------------
        std::wstring mTextureName{};
    };
}
