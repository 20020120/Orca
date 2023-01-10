#pragma once

#include"ConstantBuffer.h"
#include"DescriptorPool.h"
#include<wrl.h>
#include<DirectXMath.h>

namespace OrcaGraphics
{
    // �J�����𐧌䂷��
    class Camera
    {
    public:
        Camera() = default;
        ~Camera() = default;

        void Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, DescriptorPool* pPool_); // ������
        void Update(float Dt_); // �X�V
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_); // �`��R�}���h��ς�
        void Finalize();    // �I������
    private:
        // ------------------------------- �萔�o�b�t�@ ------------------------------
        struct alignas(256)CbData
        {
            DirectX::XMFLOAT4X4 View{};
            DirectX::XMFLOAT4X4 Proj{};
        };
        ConstantBuffer mCb{};

        DirectX::XMFLOAT3 mTarget{};
        DirectX::XMFLOAT4 mOrientation{};

    private:
        void InputMove(float Dt_); // ���͂���ړ�������
        void InputRot(float Dt_); // ���͂����]������
        const float mMoveSpeed{ 10.0f };
        const float mRotEulerSpeed{ 180.0f };
        const float mDistanceToTarget{ 10.0f };
    };
}
