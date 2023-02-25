#pragma once
#include"Vector3.h"
#include"Vector4.h"

namespace Math
{
    struct Quaternion : DirectX::XMFLOAT4
    {
        Quaternion();
        Quaternion(const Vector4& RHS_);
        Quaternion(float x, float y, float z, float w);

        // ------------------------------- ÉwÉãÉpÅ[ä÷êî ------------------------------
        [[nodiscard]] Vector3 Front()const;
        [[nodiscard]] Vector3 Up() const;
        [[nodiscard]] Vector3 Right() const;

        [[nodiscard]] static Quaternion Lerp(const Quaternion& A_, const Quaternion& B_, float Threshold_);
    };
}

