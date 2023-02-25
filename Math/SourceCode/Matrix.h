#pragma once
#include<DirectXMath.h>

#include"Vector3.h"
#include"Quaternion.h"

namespace Math
{
    struct Matrix :DirectX::XMFLOAT4X4
    {
        // --------------------------- コンストラクタ・デストラクタ --------------------------
        Matrix();
        Matrix(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);
        // ------------------------------- ヘルパー関数 ------------------------------
        [[nodiscard]] Matrix Transpose()const;

        void Clear(); // デフォルト行列にする
        // -------------------------------- 静的関数 -------------------------------
        static Matrix CreateWorld(const Vector3& Position_, const Vector3& Scale_, const Quaternion& Orientation_);

        static Matrix ComputeTranslateMatrix(const Vector3& Position_);
        static Matrix ComputeScaleMatrix(const Vector3& Scale_);
        static Matrix ComputeRotationMatrix(const Quaternion& Orientation_);
    };
}

// ------------------------------- オペレーター ------------------------------
Math::Matrix operator*(const Math::Matrix& A_, const Math::Matrix& B_);
