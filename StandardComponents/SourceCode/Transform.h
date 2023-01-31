#pragma once
#include"Component.h"
#include"MathMinimum.h"
#include"Quaternion.h"
#include"Coordinate.h"
#include<memory>
// ------------------------------ �p���𐧌䂷��R���|�\���l���g ------------------------------
namespace Component
{
    class Transform final :public Component
    {
    public:
        // ------------------------------ �R���X�g���N�^ ------------------------------
        Transform();
        explicit Transform(
            const Math::Vector3& Position_,
            const Math::Vector3& Scale_ = { 1.0f,1.0f,1.0f },
            const Math::Quaternion& Orientation_ = { 0.0f,0.0f,0.0f,1.0f });

        // ------------------------------- �f�X�g���N�^ ------------------------------
        ~Transform() override;

        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;

        // ---------------------------- ���W�n�Ɋւ���ϐ��E�֐� ---------------------------
        void SetCoordinate(Math::Coordinate Coord_);
    private:
        void ApplyCoordinate();
        Math::Coordinate mCoordinate{ Math::Coordinate::LHS_YUP };  // ���W�n���w�肷��
    public:
        Math::Vector3 mPosition{};
        Math::Vector3 mScale{ 1.0f,1.0f,1.0f };
        Math::Quaternion mOrientation{ 0.0f,0.0f,0.0f,1.0f };
        Math::Matrix mTransform{};
    private:
        bool mIsGlobal{ false };    // �s��̋�Ԃ��w�肷��
        std::shared_ptr<Transform> mpParentTransform{}; // �e�̍s��
    private:
        // ---------------------- ���������ɐe�I�u�W�F�N�g����e�̍s����������� ----------------------
        void GetParentTransform();
    private:
        bool mCheckHasParent{ false };
    };
}
