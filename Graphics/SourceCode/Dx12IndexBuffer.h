#pragma once
#include<vector>
#include<d3d12.h>
#include<wrl.h>

// ------------------------------- インデックスバッファを作成 -------------------------------
namespace OrcaGraphics
{
    namespace Resource
    {
        class IndexBuffer
        {
        public:
            // --------------------------- 頂点バッファを作成 ---------------------------
            bool Create(const std::vector<uint32_t>& Indices_);

            // ----------------------------- インデックスバッファ ----------------------------
            Microsoft::WRL::ComPtr<ID3D12Resource> mpBuffer{};
            D3D12_INDEX_BUFFER_VIEW mIbView{};
        };
    }
}
