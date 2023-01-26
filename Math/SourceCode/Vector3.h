#pragma once
#include<DirectXMath.h>

namespace Math
{
    struct Vector3 : DirectX::XMFLOAT3
    {
        Vector3();
        Vector3(float x, float y, float z);

        // ------------------------------- �w���p�[�֐� ------------------------------
        [[nodiscard]] Vector3 Normalize() const;
        [[nodiscard]] Vector3 Cross(const Vector3& RHS_)const;

        // ------------------------------ �ÓI�w���p�[�֐� -----------------------------
        [[nodiscard]] static Vector3 Cross(const Vector3& A_, const Vector3& B_);

        // ------------------------------- �I�y���[�^�[ ------------------------------
        Vector3 operator+(const Vector3& RHS_)const;
        Vector3 operator-(const Vector3& RHS_)const;
        Vector3 operator*(const Vector3& RHS_)const;
        Vector3 operator/(const Vector3& RHS_)const;

        Vector3 operator*(float RHS_)const;
        Vector3 operator/(float RHS_)const;

        Vector3& operator+=(const Vector3& RHS_);
        Vector3& operator-=(const Vector3& RHS_);
        Vector3& operator*=(const Vector3& RHS_);
        Vector3& operator/=(const Vector3& RHS_);

        Vector3& operator*=(float RHS_);
        Vector3& operator/=(float RHS_);
    };
}

// ----------------------------------- �I�y���[�^�[ ----------------------------------
Math::Vector3 operator*(float A_, const Math::Vector3& B_);