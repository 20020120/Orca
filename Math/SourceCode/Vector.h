#pragma once
#include<DirectXMath.h>
namespace Math
{
    namespace Vector
    {
        // -------------------------------- ê≥ãKâª --------------------------------
        DirectX::XMFLOAT2 Normalize(const DirectX::XMFLOAT2& Vector_);
        DirectX::XMFLOAT3 Normalize(const DirectX::XMFLOAT3& Vector_);
        DirectX::XMFLOAT4 Normalize(const DirectX::XMFLOAT4& Vector_);

        // --------------------------------- äOêœ --------------------------------
        DirectX::XMFLOAT3 Cross(const DirectX::XMFLOAT3& A_, const DirectX::XMFLOAT3& B_, bool IsNormalize_ = true);
    }
}
