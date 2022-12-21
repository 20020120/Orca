#include"pch.h"
#include"DescriptorHandle.h"
bool OrcaGraphics::DescriptorHandle::HasCPU() const
{
    return HandleCPU.ptr != 0;
}

bool OrcaGraphics::DescriptorHandle::HasGPU() const
{
    return HandleGPU.ptr != 0;
}
