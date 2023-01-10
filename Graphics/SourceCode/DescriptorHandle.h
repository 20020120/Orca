#pragma once
#include <d3d12.h>
namespace OrcaGraphics
{
    class DescriptorHandle
    {
    public:
        D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;  //!< CPUディスクリプタハンドルです.
        D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;  //!< GPUディスクリプタハンドルです.

        [[nodiscard]] bool HasCPU() const;
        [[nodiscard]] bool HasGPU() const;
    };
}