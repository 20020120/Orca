#pragma once
#include<DirectXMath.h>

#include"Vector3.h"
#include"Vector4.h"

namespace Math
{
    struct Matrix :DirectX::XMFLOAT4X4
    {
        // --------------------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------------
        Matrix();

        // ------------------------------- �w���p�[�֐� ------------------------------


        // -------------------------------- �ÓI�֐� -------------------------------
        static Matrix CreateWorld(const Vector3& Position_, const Vector3& Scale_, const Vector4& Orientation_);

        static Matrix ComputeTranslateMatrix(const Vector3& Position_);
        static Matrix ComputeScaleMatrix(const Vector3& Scale_);
        static Matrix ComputeRotationMatrix(const Vector4& Orientation_);
    };
}

// ------------------------------- �I�y���[�^�[ ------------------------------
Math::Matrix operator*(const Math::Matrix& A_, const Math::Matrix& B_);
