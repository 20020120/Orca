#include"Vector.h"

DirectX::XMFLOAT2 Math::Vector::Normalize(const DirectX::XMFLOAT2& Vector_)
{
    DirectX::XMVECTOR V = DirectX::XMLoadFloat2(&Vector_);
    V = DirectX::XMVector2Normalize(V);
    DirectX::XMFLOAT2 Ret{};
    DirectX::XMStoreFloat2(&Ret, V);
    return Ret;
}

DirectX::XMFLOAT3 Math::Vector::Normalize(const DirectX::XMFLOAT3& Vector_)
{
    DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&Vector_);
    V = DirectX::XMVector3Normalize(V);
    DirectX::XMFLOAT3 Ret{};
    DirectX::XMStoreFloat3(&Ret, V);
    return Ret;
}

DirectX::XMFLOAT4 Math::Vector::Normalize(const DirectX::XMFLOAT4& Vector_)
{
    DirectX::XMVECTOR V = DirectX::XMLoadFloat4(&Vector_);
    V = DirectX::XMVector3Normalize(V);
    DirectX::XMFLOAT4 Ret{};
    DirectX::XMStoreFloat4(&Ret, V);
    return Ret;
}

DirectX::XMFLOAT3 Math::Vector::Cross(const DirectX::XMFLOAT3& A_, const DirectX::XMFLOAT3& B_, bool IsNormalize_)
{
    const auto VA = DirectX::XMLoadFloat3(&A_);
    const auto VB = DirectX::XMLoadFloat3(&B_);
    auto Cross = DirectX::XMVector3Cross(VA, VB);
    if (IsNormalize_)
    {
        Cross = DirectX::XMVector3Normalize(Cross);
    }
    DirectX::XMFLOAT3 cross{};
    DirectX::XMStoreFloat3(&cross, Cross);
    return cross;
}
