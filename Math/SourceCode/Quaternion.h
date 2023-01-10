#pragma once
#include<DirectXMath.h>
namespace Math
{
    namespace Quaternion
    {
        DirectX::XMFLOAT3 Front(const DirectX::XMFLOAT4& Orientation_);
        DirectX::XMFLOAT3 Right(const DirectX::XMFLOAT4& Orientation_);
        DirectX::XMFLOAT3 Up(const DirectX::XMFLOAT4& Orientation_);
    }
}
