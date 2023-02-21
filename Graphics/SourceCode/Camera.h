#pragma once

#include "ConstantBuffer.h"
#include "MathMinimum.h"
#include "Quaternion.h" 

#include <wrl.h>
#include <memory>
namespace OrcaGraphics
{
    // �J�����𐧌䂷��
    class Camera
    {
    public:
        static Camera& Instance()
        {
            static Camera camera{};
            return camera;
        }
        ~Camera();

        void Initialize(); // ������
        void Update(float Dt_); // �X�V
        [[nodiscard]] uint32_t GetDescriptorIndex()const;
        void Finalize();    // �I������
    private:
        Camera() = default;

        // ------------------------------- �萔�o�b�t�@ ------------------------------
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
        void InputMove(float Dt_); // ���͂���ړ�������
        void InputRot(float Dt_); // ���͂����]������
        const float mMoveSpeed{ 10.0f };
        const float mRotEulerSpeed{ 180.0f };
        const float mDistanceToTarget{ 10.0f };
        const  float mNearClip{ 1.0f };
        const  float mFarClip{ 1000.0f };
    };
}
