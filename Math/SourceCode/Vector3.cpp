#include"pch.h"
#include"Vector3.h"

Math::Vector3::Vector3(float x, float y, float z)
    :XMFLOAT3(x,y,z)
{}

Math::Vector3::Vector3()
    :XMFLOAT3()
{}


Math::Vector3 Math::Vector3::Normalize() const
{
    using namespace DirectX;
    const auto v = XMVector3Normalize(XMLoadFloat3(this));
    Vector3 ans;
    XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::Cross(const Vector3& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat3(this);
    const auto v1 = DirectX::XMLoadFloat3(&RHS_);
    const auto v = DirectX::XMVector3Cross(v0, v1);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::Cross(const Vector3& A_, const Vector3& B_)
{
    const auto v0 = DirectX::XMLoadFloat3(&A_);
    const auto v1 = DirectX::XMLoadFloat3(&B_);
    const auto v = DirectX::XMVector3Cross(v0, v1);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::Lerp(const Vector3& A_, const Vector3& B_, float Threshold_)
{
    const auto v0 = DirectX::XMLoadFloat3(&A_);
    const auto v1 = DirectX::XMLoadFloat3(&B_);
    const auto v = DirectX::XMVectorLerp(v0, v1, Threshold_);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::operator+(const Vector3& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat3(this);
    const auto v1 = DirectX::XMLoadFloat3(&RHS_);
    const auto v = DirectX::XMVectorAdd(v0, v1);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::operator-(const Vector3& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat3(this);
    const auto v1 = DirectX::XMLoadFloat3(&RHS_);
    const auto v = DirectX::XMVectorSubtract(v0, v1);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::operator*(const Vector3& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat3(this);
    const auto v1 = DirectX::XMLoadFloat3(&RHS_);
    const auto v = DirectX::XMVectorMultiply(v0, v1);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::operator/(const Vector3& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat3(this);
    const auto v1 = DirectX::XMLoadFloat3(&RHS_);
    const auto v = DirectX::XMVectorDivide(v0, v1);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::operator*(float RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat3(this);
    const auto v = DirectX::XMVectorScale(v0, RHS_);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3 Math::Vector3::operator/(float RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat3(this);
    const auto v = DirectX::XMVectorScale(v0, 1 / RHS_);
    Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}

Math::Vector3& Math::Vector3::operator+=(const Vector3& RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat3(this);
    const XMVECTOR v2 = XMLoadFloat3(&RHS_);
    const XMVECTOR X = XMVectorAdd(v1, v2);
    XMStoreFloat3(this, X);
    return *this;
}

Math::Vector3& Math::Vector3::operator-=(const Vector3& RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat3(this);
    const XMVECTOR v2 = XMLoadFloat3(&RHS_);
    const XMVECTOR X = XMVectorSubtract(v1, v2);
    XMStoreFloat3(this, X);
    return *this;
}

Math::Vector3& Math::Vector3::operator*=(const Vector3& RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat3(this);
    const XMVECTOR v2 = XMLoadFloat3(&RHS_);
    const XMVECTOR X = XMVectorMultiply(v1, v2);
    XMStoreFloat3(this, X);
    return *this;
}

Math::Vector3& Math::Vector3::operator/=(const Vector3& RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat3(this);
    const XMVECTOR v2 = XMLoadFloat3(&RHS_);
    const XMVECTOR X = XMVectorDivide(v1, v2);
    XMStoreFloat3(this, X);
    return *this;
}

Math::Vector3& Math::Vector3::operator*=(float RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat3(this);
    const XMVECTOR X = XMVectorScale(v1, RHS_);
    XMStoreFloat3(this, X);
    return *this;
}

Math::Vector3& Math::Vector3::operator/=(float RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat3(this);
    const XMVECTOR X = XMVectorScale(v1, 1 / RHS_);
    XMStoreFloat3(this, X);
    return *this;
}

Math::Vector3 operator*(float A_, const Math::Vector3& B_)
{
    const auto v0 = DirectX::XMLoadFloat3(&B_);
    const auto v = DirectX::XMVectorScale(v0, A_);
    Math::Vector3 ans;
    DirectX::XMStoreFloat3(&ans, v);
    return ans;
}
