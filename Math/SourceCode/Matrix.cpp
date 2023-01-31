#include"pch.h"
#include"Matrix.h"

Math::Matrix::Matrix()
    :XMFLOAT4X4(
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f)
{
}

Math::Matrix Math::Matrix::Transpose() const
{
    const auto M = XMMatrixTranspose(XMLoadFloat4x4(this));
    Matrix m;
    XMStoreFloat4x4(&m, M);
    return m;
}


// ----------------------------------- ヘルパー関数 ----------------------------------

Math::Matrix Math::Matrix::CreateWorld(const Vector3& Position_, const Vector3& Scale_, const Quaternion& Orientation_)
{
    const Matrix s = ComputeScaleMatrix(Scale_);
    const Matrix r = ComputeRotationMatrix(Orientation_);
    const Matrix t = ComputeTranslateMatrix(Position_);
    return  s * r * t;
}


// ---------------------------------- 静的ヘルパー関数 ---------------------------------
Math::Matrix Math::Matrix::ComputeTranslateMatrix(const Vector3& Position_)
{
    Matrix dst{};
    const auto m = DirectX::XMMatrixTranslation(Position_.x, Position_.y, Position_.z);
    XMStoreFloat4x4(&dst, m);
    return dst;
}

Math::Matrix Math::Matrix::ComputeScaleMatrix(const Vector3& Scale_)
{
    Matrix dst{};
    const auto m = DirectX::XMMatrixScaling(Scale_.x, Scale_.y, Scale_.z);
    XMStoreFloat4x4(&dst, m);
    return dst;
}

Math::Matrix Math::Matrix::ComputeRotationMatrix(const Quaternion& Orientation_)
{
    Matrix dst{};
    const auto m = DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&Orientation_));
    XMStoreFloat4x4(&dst, m);
    return dst;
}

Math::Matrix operator*(const Math::Matrix& A_, const Math::Matrix& B_)
{
    using namespace  DirectX;
    const auto m = XMMatrixMultiply(XMLoadFloat4x4(&A_), XMLoadFloat4x4(&B_));

    Math::Matrix ans;
    XMStoreFloat4x4(&ans, m);
    return ans;
}
