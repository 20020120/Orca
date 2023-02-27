#pragma once
#include <d3d12.h>
namespace Graphics
{
    class DescriptorHandle
    {
    public:
        D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;  //!< CPU�f�B�X�N���v�^�n���h���ł�.
        D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;  //!< GPU�f�B�X�N���v�^�n���h���ł�.
        unsigned int  DescriptorIndex{};

        [[nodiscard]] bool HasCPU() const;
        [[nodiscard]] bool HasGPU() const;
    };
}