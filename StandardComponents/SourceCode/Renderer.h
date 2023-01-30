#pragma once
#include"Component.h"

#include<wrl.h>

// ------------------------------------ 前方宣言 -----------------------------------
struct ID3D12GraphicsCommandList;


// -------------------------------- 描画クラスの基底クラス --------------------------------
namespace Component
{
    class Renderer :public Component , public std::enable_shared_from_this<Renderer>
    {
    public:
        Renderer();
        ~Renderer() override = default;

        virtual void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) = 0;
    };
}
