#include"pch.h"
#include"Camera.h"
#include"VectorOperator.h"
#include"Quaternion.h"
#include "ScreenConstants.h"
Graphics::Camera::~Camera()
{
}

void Graphics::Camera::Initialize()
{
    // �萔�o�b�t�@��������
    mCb = std::make_unique<Resource::ConstantBuffer>(&mCbData);
}

void Graphics::Camera::Update(float Dt_)
{
    // �p�����X�V
    InputMove(Dt_);
    InputRot(Dt_);

    const auto normalFront = mOrientation.Front().Normalize();
    const auto eyePos = mTarget + (normalFront * mDistanceToTarget);
    const auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    constexpr auto fovY = DirectX::XMConvertToRadians(30.0f);
    constexpr auto aspect = static_cast<float>(Orca::ScreenWidth) / static_cast<float>(Orca::ScreenHeight);

    
    // �ϊ��s��̐ݒ�
    const auto V = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyePos), DirectX::XMLoadFloat3(&mTarget), upward);
    const auto P = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, mNearClip, mFarClip);
    DirectX::XMStoreFloat4x4(&mCbData->View, DirectX::XMMatrixTranspose(V));
    DirectX::XMStoreFloat4x4(&mCbData->Proj, DirectX::XMMatrixTranspose(P));
}

uint32_t Graphics::Camera::GetDescriptorIndex() const
{
    return mCb->GetDescriptorIndex();
}


void Graphics::Camera::Finalize()
{
    mCb.reset();
}

void Graphics::Camera::InputMove(float Dt_)
{
    // ���͂���J�������ړ�������֐�
    const auto front = mOrientation.Front();  // �O����
    const auto right = mOrientation.Right();  // �E����

    Math::Vector3 MoveVec{};
    // �ړ�
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

void Graphics::Camera::InputRot(float Dt_)
{
    // ----------------------------------- ��] ----------------------------------
    auto axis = DirectX::XMFLOAT2();
    if (GetAsyncKeyState('J'))  
        axis.x = -1.0f;
    if (GetAsyncKeyState('L'))  
        axis.x = 1.0f;
    if (GetAsyncKeyState('I'))  
        axis.y = -1.0f;
    if (GetAsyncKeyState('K'))  
        axis.y = 1.0f;
    // �N�H�[�^�j�I���̉�]�֐�
    auto CalcQuaRotation = [&](DirectX::XMFLOAT3 RotAxis_, const float CalculatedRotSpeed_)
    {
        const auto VUpAxis = DirectX::XMLoadFloat3(&RotAxis_);
        const auto rotQua = DirectX::XMQuaternionRotationAxis(VUpAxis, CalculatedRotSpeed_);
        auto VOri = DirectX::XMLoadFloat4(&mOrientation);
        const auto VOri2 = DirectX::XMQuaternionMultiply(VOri, rotQua);
        VOri = DirectX::XMQuaternionSlerp(VOri, VOri2, Dt_);
        DirectX::XMStoreFloat4(&mOrientation, VOri);
    };


    //--------------------<World��Up���̉�]>--------------------//
    const Math::Vector3 worldUpAxis = { 0.0f,1.0f,0.0f };

    //  ���͒l��o�ߎ��Ԃ��l���������x���Z�o
    float calculatedRotSpeed = axis.x * mRotEulerSpeed * Dt_;
    CalcQuaRotation(worldUpAxis, calculatedRotSpeed);

    //--------------------<World��X���̉�]>--------------------//

    // �J�����̑O�����Ə��������O�ςŉE�x�N�g�������߂�
    const auto cameraFront = mOrientation.Front().Normalize();
    // �O�ς̗�O����

    // �O�ς��g���ĉ�]�����Z�o
    const auto worldRightAxis = worldUpAxis.Cross(cameraFront);

    //  ���͒l��o�ߎ��Ԃ��l���������x���Z�o
    calculatedRotSpeed = axis.y * mRotEulerSpeed * Dt_;
    CalcQuaRotation(worldRightAxis, calculatedRotSpeed);

}
