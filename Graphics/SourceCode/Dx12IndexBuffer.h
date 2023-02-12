#pragma once
#include<vector>
#include<d3d12.h>
#include<wrl.h>

// ------------------------------- �C���f�b�N�X�o�b�t�@���쐬 -------------------------------
namespace OrcaGraphics
{
    namespace Resource
    {
        class IndexBuffer
        {
        public:
            // --------------------------- ���_�o�b�t�@���쐬 ---------------------------
            bool Create(const std::vector<uint32_t>& Indices_);

            // ----------------------------- �C���f�b�N�X�o�b�t�@ ----------------------------
            Microsoft::WRL::ComPtr<ID3D12Resource> mpBuffer{};
            D3D12_INDEX_BUFFER_VIEW mIbView{};
        };
    }
}
