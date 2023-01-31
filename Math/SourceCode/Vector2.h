#pragma once
#include<DirectXMath.h>

namespace Math
{
    struct Vector2 : DirectX::XMFLOAT2
    {
        Vector2();
        Vector2(float x, float y);

        // ------------------------------- �w���p�[�֐� ------------------------------
        [[nodiscard]] Vector2 Normalize() const;
        [[nodiscard]] Vector2 Cross(const Vector2& RHS_)const;

        // ------------------------------ �ÓI�w���p�[�֐� -----------------------------
        [[nodiscard]] static Vector2 Cross(const Vector2& A_, const Vector2& B_);

        // ------------------------------- �I�y���[�^�[ ------------------------------
        Vector2 operator+(const Vector2& RHS_)const;
        Vector2 operator-(const Vector2& RHS_)const;
        Vector2 operator*(const Vector2& RHS_)const;
        Vector2 operator/(const Vector2& RHS_)const;

        Vector2 operator*(float RHS_)const;
        Vector2 operator/(float RHS_)const;

        Vector2& operator+=(const Vector2& RHS_);
        Vector2& operator-=(const Vector2& RHS_);
        Vector2& operator*=(const Vector2& RHS_);
        Vector2& operator/=(const Vector2& RHS_);

        Vector2& operator*=(float RHS_);
        Vector2& operator/=(float RHS_);
    };
}

// ----------------------------------- �I�y���[�^�[ ----------------------------------
Math::Vector2 operator*(float A_, const Math::Vector2& B_);