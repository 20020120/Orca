#pragma once
#include"Dx12Resource.h"
#include"MacroMinmam.h"
#include"GraphicsMacro.h"

#include<d3d12.h>
#include<wrl.h>
#include<ResourceUploadBatch.h>

#include "Dx12ResourceInfo.h"

namespace Graphics
{
    // �O���錾
    class DescriptorPool;
    class DescriptorHandle;
    namespace Resource
    {

        class Texture final :public Dx12Resource
        {
        public:
            Texture(DirectX::ResourceUploadBatch& Batch_,const wchar_t* FileName_);
            Texture(const Texture&) = delete;
            void operator=(const Texture&) = delete;

            bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_,
                const D3D12_RESOURCE_DESC* pDesc_, bool IsCube_);

            void Load(const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);

        private:
            // ------------------------------- �w���p�[�֐� ------------------------------
            void LoadTextureFromFile(OrcaComPtr(ID3D12Device) pDevice_, const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);

            // DDS�t�@�C�����烍�[�h����
            void DDSLoadTexture(OrcaComPtr(ID3D12Device) pDevice_, const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);
            // WIC�t�@�C�����烍�[�h����
            void WICLoadTexture(OrcaComPtr(ID3D12Device) pDevice_, const wchar_t* FileName_, DirectX::ResourceUploadBatch& Batch_);
            OrcaNodiscard D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool IsCube_) const;
        };
    }
}
