#include"pch.h"
#include "VectorOperator.h"

DirectX::XMFLOAT3 MathHelper::Operator::operator*(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
   
    const DirectX::XMVECTOR A = XMLoadFloat3(&a);
    const DirectX::XMVECTOR B = XMLoadFloat3(&b);
    const DirectX::XMVECTOR Ans = DirectX::XMVectorMultiply(A, B);
    DirectX::XMFLOAT3 ret{};
    XMStoreFloat3(&ret, Ans);
    return ret;
}

DirectX::XMFLOAT3 MathHelper::Operator::operator*(float a, const DirectX::XMFLOAT3& b)
{
    return { b.x * a,b.y * a, b.z * a, };
}

DirectX::XMFLOAT3 MathHelper::Operator::operator*(const DirectX::XMFLOAT3& a, float b)
{
    return { a.x * b,a.y * b, a.z * b, };
}

DirectX::XMFLOAT3 MathHelper::Operator::operator+(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    return { a.x + b.x,a.y + b.y, a.z + b.z };
}

void MathHelper::Operator::operator+=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    using namespace DirectX;
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}

DirectX::XMFLOAT3 MathHelper::Operator::operator-(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    return { a.x - b.x,a.y - b.y, a.z - b.z };
}

