#include"pch.h"
#include"Camera.h"
#include"VectorOperator.h"
#include"Quaternion.h"
#include"Vector.h"
#include "ScreenConstants.h"

OrcaGraphics::Camera::~Camera()
{
    mCb.Finalize();
}

void OrcaGraphics::Camera::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_)
{
    // �萔�o�b�t�@��������
    mCb.Initialize(pDevice_, pPool_, sizeof(CbData));
}

void OrcaGraphics::Camera::Update(float Dt_)
{
    using namespace Math::Operator;

    // �p�����X�V
    InputMove(Dt_);
    InputRot(Dt_);

    const auto normalFront = Math::Vector::Normalize(Math::Quaternion::Front(mOrientation));
    const auto eyePos = mTarget + (normalFront * mDistanceToTarget);
    const auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    constexpr auto fovY = DirectX::XMConvertToRadians(30.0f);
    constexpr float aspect = static_cast<float>(Orca::ScreenWidth) / static_cast<float>(Orca::ScreenHeight);

    // �ϊ��s��̐ݒ�
    const auto pData = mCb.GetPtr<CbData>();
    const auto V = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eyePos), DirectX::XMLoadFloat3(&mTarget), upward);
    const auto P = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, mNearClip, mFarClip);
    DirectX::XMStoreFloat4x4(&pData->View, DirectX::XMMatrixTranspose(V));
    DirectX::XMStoreFloat4x4(&pData->Proj, DirectX::XMMatrixTranspose(P));
}

void OrcaGraphics::Camera::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    // �萔�f�[�^���o�C���h
    pCmdList_->SetGraphicsRootDescriptorTable(0, mCb.GetGPU());
}
void OrcaGraphics::Camera::Finalize()
{
}

void OrcaGraphics::Camera::InputMove(float Dt_)
{
    using namespace Math::Operator;

    // ���͂���J�������ړ�������֐�
    auto front = Math::Quaternion::Front(mOrientation); // �O����
    auto right = Math::Quaternion::Right(mOrientation); // �E����

    DirectX::XMFLOAT3 MoveVec{};
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

    mTarget += mMoveSpeed * Dt_ * MoveVec;
}

void OrcaGraphics::Camera::InputRot(float Dt_)
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
    const DirectX::XMFLOAT3 worldUpAxis = { 0.0f,1.0f,0.0f };

    //  ���͒l��o�ߎ��Ԃ��l���������x���Z�o
    float calculatedRotSpeed = axis.x * mRotEulerSpeed * Dt_;
    CalcQuaRotation(worldUpAxis, calculatedRotSpeed);

    //--------------------<World��X���̉�]>--------------------//

    // �J�����̑O�����Ə��������O�ςŉE�x�N�g�������߂�
    DirectX::XMFLOAT3 cameraFront = Math::Quaternion::Front(mOrientation);
    cameraFront = Math::Vector::Normalize(cameraFront);
    // �O�ς̗�O����

    // �O�ς��g���ĉ�]�����Z�o
    const DirectX::XMFLOAT3 worldRightAxis = Math::Vector::Cross(worldUpAxis, cameraFront, false);

    //  ���͒l��o�ߎ��Ԃ��l���������x���Z�o
    calculatedRotSpeed = axis.y * mRotEulerSpeed * Dt_;
    CalcQuaRotation(worldRightAxis, calculatedRotSpeed);

}
