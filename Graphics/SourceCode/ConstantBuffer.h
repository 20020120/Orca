#pragma once
#include<d3d12.h>
#include"MacroMinmam.h"
#include"GraphicsMacro.h"
#include"RenderResource.h"
namespace OrcaGraphics
{
    // ëOï˚êÈåæ
    class DescriptorPool;
    class DescriptorHandle;

    class ConstantBuffer :public RenderResource
    {
    public:
        ConstantBuffer(OrcaComPtr(ID3D12Device) pDevice_, DescriptorPool* pPool_, size_t Size_, UINT RootParamIndex_, void** MappedPtr_);
        ~ConstantBuffer() override = default;
        ConstantBuffer(const ConstantBuffer&) = delete;
        void operator =(const ConstantBuffer&) = delete;

        void Finalize();
    void* p{};
    private:
        D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc{};
    };

}