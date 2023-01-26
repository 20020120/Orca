#pragma once
#include<DirectXMath.h>
namespace Math
{
    struct Vector4 : DirectX::XMFLOAT4
    {
        Vector4();
        Vector4(float x, float y, float z, float w);
    };
}
