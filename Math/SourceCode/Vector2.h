#pragma once
#include<DirectXMath.h>

namespace Math
{
    struct Vector2 : DirectX::XMFLOAT2
    {
        Vector2();
        Vector2(float x, float y);

        // ------------------------------- ヘルパー関数 ------------------------------
        [[nodiscard]] Vector2 Normalize() const;
        [[nodiscard]] Vector2 Cross(const Vector2& RHS_)const;

        // ------------------------------ 静的ヘルパー関数 -----------------------------
        [[nodiscard]] static Vector2 Cross(const Vector2& A_, const Vector2& B_);

        // ------------------------------- オペレーター ------------------------------
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

// ----------------------------------- オペレーター ----------------------------------
Math::Vector2 operator*(float A_, const Math::Vector2& B_);