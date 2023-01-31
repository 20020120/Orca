#pragma once
#include"Component.h"
#include"MathMinimum.h"
#include"Quaternion.h"
#include"Coordinate.h"
#include<memory>
// ------------------------------ 姿勢を制御するコンポ―ンネント ------------------------------
namespace Component
{
    class Transform final :public Component
    {
    public:
        // ------------------------------ コンストラクタ ------------------------------
        Transform();
        explicit Transform(
            const Math::Vector3& Position_,
            const Math::Vector3& Scale_ = { 1.0f,1.0f,1.0f },
            const Math::Quaternion& Orientation_ = { 0.0f,0.0f,0.0f,1.0f });

        // ------------------------------- デストラクタ ------------------------------
        ~Transform() override;

        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;

        // ---------------------------- 座標系に関する変数・関数 ---------------------------
        void SetCoordinate(Math::Coordinate Coord_);
    private:
        void ApplyCoordinate();
        Math::Coordinate mCoordinate{ Math::Coordinate::LHS_YUP };  // 座標系を指定する
    public:
        Math::Vector3 mPosition{};
        Math::Vector3 mScale{ 1.0f,1.0f,1.0f };
        Math::Quaternion mOrientation{ 0.0f,0.0f,0.0f,1.0f };
        Math::Matrix mTransform{};
    private:
        bool mIsGlobal{ false };    // 行列の空間を指定する
        std::shared_ptr<Transform> mpParentTransform{}; // 親の行列
    private:
        // ---------------------- 初期化時に親オブジェクトから親の行列を検索する ----------------------
        void GetParentTransform();
    private:
        bool mCheckHasParent{ false };
    };
}
