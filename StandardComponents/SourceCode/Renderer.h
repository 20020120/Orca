#pragma once
#include"Component.h"

#include<wrl.h>

// ------------------------------------ �O���錾 -----------------------------------
struct ID3D12GraphicsCommandList;


// -------------------------------- �`��N���X�̊��N���X --------------------------------
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
