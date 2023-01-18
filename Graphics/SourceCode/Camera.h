#pragma once

#include"ConstantBuffer.h"
#include"DescriptorPool.h"
#include<wrl.h>
#include<DirectXMath.h>
#include<memory>
namespace OrcaGraphics
{
    // カメラを制御する
    class Camera
    {
    public:
        Camera() = default;
        ~Camera();

        void Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_); // 初期化
        void Update(float Dt_); // 更新
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) const; // 描画コマンドを積む
        void Finalize();    // 終了処理
    private:
        // ------------------------------- 定数バッファ ------------------------------
        struct alignas(256)CbData
        {
            DirectX::XMFLOAT4X4 View{};
            DirectX::XMFLOAT4X4 Proj{};
        };
        std::unique_ptr<ConstantBuffer> mCb{};
        CbData* mCbData{};

        DirectX::XMFLOAT3 mTarget{};
        DirectX::XMFLOAT4 mOrientation{ 0.0f,0.0f,0.0f,1.0f };

    private:
        void InputMove(float Dt_); // 入力から移動させる
        void InputRot(float Dt_); // 入力から回転させる
        const float mMoveSpeed{ 10.0f };
        const float mRotEulerSpeed{ 180.0f };
        const float mDistanceToTarget{ 10.0f };
        const  float mNearClip{ 1.0f };
        const  float mFarClip{ 1000.0f };
    };
}
