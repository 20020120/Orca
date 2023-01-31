#include"pch.h"
#include"Vector2.h"

Math::Vector2::Vector2(float x, float y)
    :XMFLOAT2(x, y)
{}

Math::Vector2::Vector2()
    :XMFLOAT2()
{}


Math::Vector2 Math::Vector2::Normalize() const
{
    using namespace DirectX;
    const auto v = XMVector2Normalize(XMLoadFloat2(this));
    Vector2 ans;
    XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2 Math::Vector2::Cross(const Vector2& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat2(this);
    const auto v1 = DirectX::XMLoadFloat2(&RHS_);
    const auto v = DirectX::XMVector2Cross(v0, v1);
    Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2 Math::Vector2::Cross(const Vector2& A_, const Vector2& B_)
{
    const auto v0 = DirectX::XMLoadFloat2(&A_);
    const auto v1 = DirectX::XMLoadFloat2(&B_);
    const auto v = DirectX::XMVector2Cross(v0, v1);
    Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2 Math::Vector2::operator+(const Vector2& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat2(this);
    const auto v1 = DirectX::XMLoadFloat2(&RHS_);
    const auto v = DirectX::XMVectorAdd(v0, v1);
    Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2 Math::Vector2::operator-(const Vector2& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat2(this);
    const auto v1 = DirectX::XMLoadFloat2(&RHS_);
    const auto v = DirectX::XMVectorSubtract(v0, v1);
    Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2 Math::Vector2::operator*(const Vector2& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat2(this);
    const auto v1 = DirectX::XMLoadFloat2(&RHS_);
    const auto v = DirectX::XMVectorMultiply(v0, v1);
    Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2 Math::Vector2::operator/(const Vector2& RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat2(this);
    const auto v1 = DirectX::XMLoadFloat2(&RHS_);
    const auto v = DirectX::XMVectorDivide(v0, v1);
    Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2 Math::Vector2::operator*(float RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat2(this);
    const auto v = DirectX::XMVectorScale(v0, RHS_);
    Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2 Math::Vector2::operator/(float RHS_) const
{
    const auto v0 = DirectX::XMLoadFloat2(this);
    const auto v = DirectX::XMVectorScale(v0, 1 / RHS_);
    Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}

Math::Vector2& Math::Vector2::operator+=(const Vector2& RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&RHS_);
    const XMVECTOR X = XMVectorAdd(v1, v2);
    XMStoreFloat2(this, X);
    return *this;
}

Math::Vector2& Math::Vector2::operator-=(const Vector2& RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&RHS_);
    const XMVECTOR X = XMVectorSubtract(v1, v2);
    XMStoreFloat2(this, X);
    return *this;
}

Math::Vector2& Math::Vector2::operator*=(const Vector2& RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&RHS_);
    const XMVECTOR X = XMVectorMultiply(v1, v2);
    XMStoreFloat2(this, X);
    return *this;
}

Math::Vector2& Math::Vector2::operator/=(const Vector2& RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&RHS_);
    const XMVECTOR X = XMVectorDivide(v1, v2);
    XMStoreFloat2(this, X);
    return *this;
}

Math::Vector2& Math::Vector2::operator*=(float RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR X = XMVectorScale(v1, RHS_);
    XMStoreFloat2(this, X);
    return *this;
}

Math::Vector2& Math::Vector2::operator/=(float RHS_)
{
    using namespace DirectX;
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR X = XMVectorScale(v1, 1 / RHS_);
    XMStoreFloat2(this, X);
    return *this;
}

Math::Vector2 operator*(float A_, const Math::Vector2& B_)
{
    const auto v0 = DirectX::XMLoadFloat2(&B_);
    const auto v = DirectX::XMVectorScale(v0, A_);
    Math::Vector2 ans;
    DirectX::XMStoreFloat2(&ans, v);
    return ans;
}
