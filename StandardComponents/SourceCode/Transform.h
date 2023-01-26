#pragma once
#include"Component.h"
#include"MathMinimum.h"


// ------------------------------ �p���𐧌䂷��R���|�\���l���g ------------------------------
namespace Component
{
    class Transform final :public Component
    {
    public:
        Transform();
        ~Transform() override;

        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;

    private:
        Math::Vector3 mPosition{};
        Math::Vector3 mScale{ 1.0f,1.0f,1.0f };
        Math::Vector4 mOrientation{ 0.0f,0.0f,0.0f,1.0f };
        Math::Matrix mTransform{};

        bool mIsGlobal{ false };    // �s��̋�Ԃ��w�肷��
        std::shared_ptr<Transform> mpParentTransform{}; // �e�̍s��
    };
}
