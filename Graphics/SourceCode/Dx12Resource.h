#pragma once
//#include <d3d12.h>
#include"GraphicsMacro.h"
#include<string>

struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

namespace OrcaGraphics
{
    // ---------------------------------- 前方宣言 ---------------------------------
    class DescriptorPool;
    class DescriptorHandle;

    namespace Resource
    {
        // ---------------------------------- 描画用のリソースの基底クラス ---------------------------------
        class Dx12Resource
        {
        public:
            // 共通の初期化
            Dx12Resource(std::string Name_, DescriptorPool* pDescriptorPool_, UINT RootParamIndex);
            virtual ~Dx12Resource();

            void Bind(OrcaComPtr(ID3D12GraphicsCommandList) pCmdList_) const; // GPUにバインド
            std::string GetName()const;
        private:
            UINT mRootParamIndex{}; // ルートパラメーターの番号
        protected:
            DescriptorHandle* mpHandle{};
            DescriptorPool* mpPool{};
            OrcaComPtr(ID3D12Resource) mpResource {};
            std::string mName{};
        };
    }
}

