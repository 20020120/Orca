#pragma once
#include<DirectXMath.h>
namespace Math
{
    namespace Vector
    {
        // -------------------------------- ���K�� --------------------------------
        DirectX::XMFLOAT2 Normalize(const DirectX::XMFLOAT2& Vector_);
        DirectX::XMFLOAT3 Normalize(const DirectX::XMFLOAT3& Vector_);
        DirectX::XMFLOAT4 Normalize(const DirectX::XMFLOAT4& Vector_);

        // --------------------------------- �O�� --------------------------------
        DirectX::XMFLOAT3 Cross(const DirectX::XMFLOAT3& A_, const DirectX::XMFLOAT3& B_, bool IsNormalize_ = true);
    }
}
