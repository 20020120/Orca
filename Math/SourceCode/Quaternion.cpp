#include"pch.h"
#include"Quaternion.h"

Math::Quaternion::Quaternion()
    :XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
{}

Math::Quaternion::Quaternion(const Vector4& RHS_)
    :XMFLOAT4(RHS_)
{}

Math::Quaternion::Quaternion(float x, float y, float z, float w)
    :XMFLOAT4(x,y,z,w)
{}

Math::Vector3 Math::Quaternion::Front() const
{
    const DirectX::XMVECTOR O = XMLoadFloat4(this);
    const DirectX::XMMATRIX R{ DirectX::XMMatrixRotationQuaternion(O) };
    Vector3 front;
    XMStoreFloat3(&front, R.r[2]);
    return front;
}

Math::Vector3 Math::Quaternion::Up() const
{
    const DirectX::XMVECTOR O = XMLoadFloat4(this);
    const DirectX::XMMATRIX R{ DirectX::XMMatrixRotationQuaternion(O) };
    Vector3 up;
    XMStoreFloat3(&up, R.r[1]);
    return up;
}

Math::Vector3 Math::Quaternion::Right() const
{
    const DirectX::XMVECTOR O = XMLoadFloat4(this);
    const DirectX::XMMATRIX R{ DirectX::XMMatrixRotationQuaternion(O) };
    Vector3 right;
    XMStoreFloat3(&right, R.r[0]);
    return right;
}
