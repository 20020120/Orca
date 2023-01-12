#pragma once
#include<d3d12.h>
#include<wrl.h>
#include<ResourceUploadBatch.h>
#include"MacroMinmam.h"
#include"GraphicsMacro.h"

namespace OrcaGraphics
{
    // �O���錾
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
    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> mpResource{};
        DescriptorHandle* mpHandle{};
        DescriptorPool* mpPool{};

        // ------------------------------- �w���p�[�֐� ------------------------------
        void LoadTextureFromFile(OrcaComPtr(ID3D12Device) pDevice_, const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);

        // DDS�t�@�C�����烍�[�h����
        void DDSLoadTexture(OrcaComPtr(ID3D12Device) pDevice_, const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);
        // WIC�t�@�C�����烍�[�h����
        void WICLoadTexture(OrcaComPtr(ID3D12Device) pDevice_,const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);
        OrcaNodiscard D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool IsCube_) const;
    };
}