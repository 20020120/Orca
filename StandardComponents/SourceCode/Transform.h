#pragma once
#include"Component.h"
#include"MathMinimum.h"
#include"Quaternion.h"

// ------------------------------ �p���𐧌䂷��R���|�\���l���g ------------------------------
namespace Component
{
    class Transform final :public Component
    {
    public:
        // ------------------------------ �R���X�g���N�^ ------------------------------
        Transform();
        Transform(
            const Math::Vector3& Position_,
            const Math::Vector3& Scale_ = { 1.0f,1.0f,1.0f },
            const Math::Quaternion& Orientation_ = { 0.0f,0.0f,0.0f,1.0f });

        // ------------------------------- �f�X�g���N�^ ------------------------------
        ~Transform() override;

        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;

    private:
        Math::Vector3 mPosition{};
        Math::Vector3 mScale{ 1.0f,1.0f,1.0f };
        Math::Quaternion mOrientation{ 0.0f,0.0f,0.0f,1.0f };
        Math::Matrix mTransform{};

        bool mIsGlobal{ false };    // �s��̋�Ԃ��w�肷��
        std::shared_ptr<Transform> mpParentTransform{}; // �e�̍s��
    };
}
