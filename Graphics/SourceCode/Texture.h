#pragma once
#include"Dx12Resource.h"
#include"MacroMinmam.h"
#include"GraphicsMacro.h"

#include<d3d12.h>
#include<wrl.h>
#include<ResourceUploadBatch.h>
namespace OrcaGraphics
{
    // 前方宣言
    class DescriptorPool;
    class DescriptorHandle;
    namespace Resource
    {

        class Texture :public Dx12Resource
        {
        public:
            Texture();
            Texture(const Texture&) = delete;
            void operator=(const Texture&) = delete;

            bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_,
                const D3D12_RESOURCE_DESC* pDesc_, bool IsCube_);
            bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_,
                const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);

            OrcaNodiscard D3D12_CPU_DESCRIPTOR_HANDLE GetHandleCPU()const;
            OrcaNodiscard D3D12_GPU_DESCRIPTOR_HANDLE GetHandleGPU()const;
        private:
            Microsoft::WRL::ComPtr<ID3D12Resource> mpResource{};
            DescriptorHandle* mpHandle{};
            DescriptorPool* mpPool{};

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