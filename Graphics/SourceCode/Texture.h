#pragma once
#include<d3d12.h>
#include<wrl.h>
#include"MacroMinmam.h"
#include<ResourceUploadBatch.h>

namespace OrcaGraphics
{
    // ëOï˚êÈåæ
    class DescriptorPool;
    class DescriptorHandle;

    class Texture
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
        OrcaNodiscard D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool IsCube_)const;

    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> mpResource{};
        DescriptorHandle* mpHandle{};
        DescriptorPool* mpPool{};
    };
}