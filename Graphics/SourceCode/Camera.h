#pragma once

#include "ConstantBuffer.h"
#include "MathMinimum.h"
#include "Quaternion.h" 

#include <wrl.h>
#include <memory>
namespace OrcaGraphics
{
    // カメラを制御する
    class Camera
    {
    public:
        static Camera& Instance()
        {
            static Camera camera{};
            return camera;
        }
        ~Camera();

        void Initialize(); // 初期化
        void Update(float Dt_); // 更新
        [[nodiscard]] uint32_t GetDescriptorIndex()const;
        void Finalize();    // 終了処理
    private:
        Camera() = default;

        // ------------------------------- 定数バッファ ------------------------------
        struct alignas(256)CbData
        {
            Math::Matrix View{};
            Math::Matrix Proj{};
        };
        std::unique_ptr<Resource::ConstantBuffer> mCb{};
        CbData* mCbData{};

        Math::Vector3 mTarget{};
        Math::Quaternion mOrientation{};

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
