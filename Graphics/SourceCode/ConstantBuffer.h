#pragma once
#include<d3d12.h>
#include"GraphicsMacro.h"
#include"Dx12Resource.h"
namespace OrcaGraphics
{
    // ëOï˚êÈåæ
    class DescriptorPool;
    class DescriptorHandle;

    namespace Resource
    {
        class ConstantBuffer :public Dx12Resource
        {
        public:
            ConstantBuffer(OrcaComPtr(ID3D12Device) pDevice_, DescriptorPool* pPool_, size_t Size_, UINT RootParamIndex_);
            ~ConstantBuffer() override = default;
            ConstantBuffer(const ConstantBuffer&) = delete;
            void operator =(const ConstantBuffer&) = delete;

            void Mapping(void** Ptr_) const;
        private:
            void Initialize(OrcaComPtr(ID3D12Device) pDevice_, DescriptorPool* pPool_, size_t Size_);
            D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc{};
        };
    }
}

