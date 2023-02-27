#pragma once
#include<d3d12.h>
#include"GraphicsMacro.h"
#include"Dx12Resource.h"
#include"Graphics.h"
namespace Graphics
{
    // ëOï˚êÈåæ
    class DescriptorPool;
    class DescriptorHandle;

    namespace Resource
    {
        class ConstantBuffer final :public Dx12Resource
        {
        public:
            template<class T>
            ConstantBuffer(T** pBuffer_);
            ~ConstantBuffer() override = default;
            ConstantBuffer(const ConstantBuffer&) = delete;
            void operator =(const ConstantBuffer&) = delete;

        private:
            void Initialize(OrcaComPtr(ID3D12Device) pDevice_, const DescriptorPool* pPool_, size_t Size_);
            void Mapping(void** Ptr_) const;

            D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc{};
        };
    }

    template<class T>
    Resource::ConstantBuffer::ConstantBuffer(T** pBuffer_)
        :Dx12Resource(Graphics::GetDescriptorPool(POOL_TYPE_RES))
    {
        Initialize(Graphics::GetDevice(), Graphics::GetDescriptorPool(POOL_TYPE_RES), sizeof(T));
        Mapping(reinterpret_cast<void**>(pBuffer_));
    }
}

