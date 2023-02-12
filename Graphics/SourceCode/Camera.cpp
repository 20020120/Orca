#include"pch.h"
#include"Camera.h"
#include"VectorOperator.h"
#include"Quaternion.h"
#include"Vector.h"
#include "ScreenConstants.h"
OrcaGraphics::Camera::~Camera()
{
}

void OrcaGraphics::Camera::Initialize()
{
    // 定数バッファを初期化
    mCb = std::make_unique<Resource::ConstantBuffer>("Camera", sizeof(CbData), 0);
    mCb->Mapping(reinterpret_cast<void**>(&mCbData));
}

void OrcaGraphics::Camera::Update(float Dt_)
{
    // 姿勢を更新
    InputMove(Dt_);
    InputRot(Dt_);

    const auto normalFront = mOrientation.Front().Normalize();
    const auto eyePos = mTarget + (normalFront * mDistanceToTarget);
    const auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    constexpr auto fovY = DirectX::XMConvertToRadians(30.0f);
    constexpr auto aspect = static_cast<float>(Orca::ScreenWidth) / static_cast<float>(Orca::ScreenHeight);

    
    // 変換行列の設定
    const auto V = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyePos), DirectX::XMLoadFloat3(&mTarget), upward);
    const auto P = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, mNearClip, mFarClip);
    DirectX::XMStoreFloat4x4(&mCbData->View, DirectX::XMMatrixTranspose(V));
    DirectX::XMStoreFloat4x4(&mCbData->Proj, DirectX::XMMatrixTranspose(P));
}

void OrcaGraphics::Camera::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) const
{
    mCb->Bind(pCmdList_);
}

void OrcaGraphics::Camera::Finalize()
{
    mCb.reset();
}

void OrcaGraphics::Camera::InputMove(float Dt_)
{
    // 入力からカメラを移動させる関数
    const auto front = mOrientation.Front();  // 前方向
    const auto right = mOrientation.Right();  // 右方向

    Math::Vector3 MoveVec{};
    // 移動
    if (GetAsyncKeyState('W')) {
        MoveVec = front;
    }
    if (GetAsyncKeyState('S')) {
        MoveVec = front * -1.0f;
    }

    if (GetAsyncKeyState('A')) {
        MoveVec = right * -1.0f;
    }

    if (GetAsyncKeyState('D')) {
        MoveVec = right;
    }

    if (GetAsyncKeyState('Q')) {
        MoveVec.y = 1.0f;
    }

    if (GetAsyncKeyState('E')) {
        MoveVec.y = -1.0f;
    }

    mTarget += MoveVec * mMoveSpeed * Dt_;
}

void OrcaGraphics::Camera::InputRot(float Dt_)
{
    // ----------------------------------- 回転 ----------------------------------
    auto axis = DirectX::XMFLOAT2();
    if (GetAsyncKeyState('J'))  
        axis.x = -1.0f;
    if (GetAsyncKeyState('L'))  
        axis.x = 1.0f;
    if (GetAsyncKeyState('I'))  
        axis.y = -1.0f;
    if (GetAsyncKeyState('K'))  
        axis.y = 1.0f;
    // クォータニオンの回転関数
    auto CalcQuaRotation = [&](DirectX::XMFLOAT3 RotAxis_, const float CalculatedRotSpeed_)
    {
        const auto VUpAxis = DirectX::XMLoadFloat3(&RotAxis_);
        const auto rotQua = DirectX::XMQuaternionRotationAxis(VUpAxis, CalculatedRotSpeed_);
        auto VOri = DirectX::XMLoadFloat4(&mOrientation);
        const auto VOri2 = DirectX::XMQuaternionMultiply(VOri, rotQua);
        VOri = DirectX::XMQuaternionSlerp(VOri, VOri2, Dt_);
        DirectX::XMStoreFloat4(&mOrientation, VOri);
    };


    //--------------------<WorldのUp軸の回転>--------------------//
    const Math::Vector3 worldUpAxis = { 0.0f,1.0f,0.0f };

    //  入力値や経過時間を考慮した速度を算出
    float calculatedRotSpeed = axis.x * mRotEulerSpeed * Dt_;
    CalcQuaRotation(worldUpAxis, calculatedRotSpeed);

    //--------------------<WorldのX軸の回転>--------------------//

    // カメラの前方向と上方向から外積で右ベクトルを求める
    const auto cameraFront = mOrientation.Front().Normalize();
    // 外積の例外処理

    // 外積を使って回転軸を算出
    const auto worldRightAxis = worldUpAxis.Cross(cameraFront);

    //  入力値や経過時間を考慮した速度を算出
    calculatedRotSpeed = axis.y * mRotEulerSpeed * Dt_;
    CalcQuaRotation(worldRightAxis, calculatedRotSpeed);

}
