#pragma once
#include<DirectXMath.h>
namespace MathHelper
{
    namespace Operator
    {
        // -------------------------------- �����Z --------------------------------
        DirectX::XMFLOAT3 operator + (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
        void operator += (DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

        // -------------------------------- �����Z --------------------------------
        DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

        // -------------------------------- �|���Z --------------------------------
        DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
        DirectX::XMFLOAT3 operator * (float a, const DirectX::XMFLOAT3& b);
        DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3& a, float b);

        // -------------------------------- ����Z --------------------------------

    }
}
