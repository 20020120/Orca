#pragma once
#include"Dx12Resource.h"
#include"MacroMinmam.h"
#include"GraphicsMacro.h"

#include<d3d12.h>
#include<wrl.h>
#include<ResourceUploadBatch.h>

#include "Dx12ResourceInfo.h"

namespace OrcaGraphics
{
    // 前方宣言
    class DescriptorPool;
    class DescriptorHandle;
    namespace Resource
    {

        class Texture final :public Dx12Resource
        {
        public:
            Texture(std::string Name_, uint32_t RootIndex_);
            Texture(const Texture&) = delete;
            void operator=(const Texture&) = delete;

            bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_,
                const D3D12_RESOURCE_DESC* pDesc_, bool IsCube_);

            void Load(const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);

        private:
            // ------------------------------- ヘルパー関数 ------------------------------
            void LoadTextureFromFile(OrcaComPtr(ID3D12Device) pDevice_, const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);

            // DDSファイルからロードする
            void DDSLoadTexture(OrcaComPtr(ID3D12Device) pDevice_, const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);
            // WICファイルからロードする
            void WICLoadTexture(OrcaComPtr(ID3D12Device) pDevice_, const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);
            OrcaNodiscard D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool IsCube_) const;
        };
    }
}
