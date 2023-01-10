#include"Quaternion.h"

DirectX::XMFLOAT3 Math::Quaternion::Front(const DirectX::XMFLOAT4& Orientation_)
{
    const DirectX::XMVECTOR O = DirectX::XMLoadFloat4(&Orientation_);
    const DirectX::XMMATRIX R{ DirectX::XMMatrixRotationQuaternion(O) };
    DirectX::XMFLOAT3 front{};
    DirectX::XMStoreFloat3(&front, R.r[2]);
    return front;
}

DirectX::XMFLOAT3 Math::Quaternion::Right(const DirectX::XMFLOAT4& Orientation_)
{
    const DirectX::XMVECTOR O = DirectX::XMLoadFloat4(&Orientation_);
    const DirectX::XMMATRIX R{ DirectX::XMMatrixRotationQuaternion(O) };
    DirectX::XMFLOAT3 right{};
    DirectX::XMStoreFloat3(&right, R.r[0]);
    return right;
}

DirectX::XMFLOAT3 Math::Quaternion::Up(const DirectX::XMFLOAT4& Orientation_)
{
    const DirectX::XMVECTOR O = DirectX::XMLoadFloat4(&Orientation_);
    const DirectX::XMMATRIX R{ DirectX::XMMatrixRotationQuaternion(O) };
    DirectX::XMFLOAT3 up{};
    DirectX::XMStoreFloat3(&up, R.r[1]);
    return up;
}
