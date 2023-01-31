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

        // ------------------------------- ヘルパー関数 ------------------------------
        Matrix Transpose()const;

        // -------------------------------- 静的関数 -------------------------------
        static Matrix CreateWorld(const Vector3& Position_, const Vector3& Scale_, const Quaternion& Orientation_);

        static Matrix ComputeTranslateMatrix(const Vector3& Position_);
        static Matrix ComputeScaleMatrix(const Vector3& Scale_);
        static Matrix ComputeRotationMatrix(const Quaternion& Orientation_);
    };
}

// ------------------------------- オペレーター ------------------------------
Math::Matrix operator*(const Math::Matrix& A_, const Math::Matrix& B_);
