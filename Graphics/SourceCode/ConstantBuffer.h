#pragma once
#include<d3d12.h>
#include"MacroMinmam.h"
#include"GraphicsMacro.h"
namespace OrcaGraphics
{
    // ëOï˚êÈåæ
    class DescriptorPool;
    class DescriptorHandle;

    class ConstantBuffer
    {
    public:
        ConstantBuffer() = default;
        ~ConstantBuffer();
        ConstantBuffer(const ConstantBuffer&) = delete;
        void operator =(const ConstantBuffer&) = delete;

        bool Initialize(OrcaComPtr(ID3D12Device) pDevice_, DescriptorPool* pPool_, size_t Size_);
        void Finalize();

        OrcaNodiscard D3D12_GPU_VIRTUAL_ADDRESS GetAddress()const;
        OrcaNodiscard D3D12_CPU_DESCRIPTOR_HANDLE GetCPU()const;
        OrcaNodiscard D3D12_GPU_DESCRIPTOR_HANDLE GetGPU()const;
        void* GetPtr();
        template<class T>
        T* GetPtr();

    private:
        D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc{};
        OrcaComPtr(ID3D12Resource) mpCb {};
        DescriptorHandle* mpHandle{};
        DescriptorPool* mpPool{};
        void* mpMappedPtr{};
    };

    
}
