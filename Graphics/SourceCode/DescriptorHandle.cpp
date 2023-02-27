#include"pch.h"
#include"DescriptorHandle.h"
bool Graphics::DescriptorHandle::HasCPU() const
{
    return HandleCPU.ptr != 0;
}

bool Graphics::DescriptorHandle::HasGPU() const
{
    return HandleGPU.ptr != 0;
}
