#pragma once
#include <cstdint>
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>
#include<DirectXMath.h>

#include "ConstantBuffer.h"
#include"ScreenConstants.h"
#include "Texture.h"


namespace OrcaGraphics
{
    // DirectX12�̕`��N���X
    class Graphics
    {
    public:
        enum POOL_TYPE
        {
            POOL_TYPE_RES = 0,     // CBV / SRV / UAV
            POOL_TYPE_SMP = 1,     // Sampler
            POOL_TYPE_RTV = 2,     // RTV
            POOL_TYPE_DSV = 3,     // DSV
            POOL_COUNT = 4,
        };
    public:
        Graphics();
        ~Graphics();

        void Initialize(HWND hWnd_);  // ������
        void Finalize(); // �I������
        void OnTerm();  // �I�����̏���

        void OpenCmdList();
        void CloseCmdList();

        void StackCmdList();

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Device> GetDevice()const;
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCmdList()const;
        [[nodiscard]] DescriptorPool* GetDescriptorPool(POOL_TYPE Type_)const;
        [[nodiscard]] OrcaComPtr(ID3D12CommandQueue) GetCommandQueue()const;
    private:
        void CreateDevice();                  // �f�o�C�X�̏�����
        void CreateCommandQueue();            // �R�}���h�L���[�̏�����
        void CreateSwapChain(HWND hWnd_);     // �X���b�v�`�F�[���̍쐬
        void CreateCommandAllocator();        // �R�}���h�A���P�[�^�[���쐬
        void CreateCommandList();             // �R�}���h���X�g���쐬
        void CreateRenderTargetView();        // �����_�[�^�[�Q�b�g�r���[���쐬
        void CreateFence();                   // �t�F���X�I�u�W�F�N�g���쐬����
        void CreateDepthBuffer();             // �f�v�X�o�b�t�@���쐬

        // ----------------------------------- �ϐ� ----------------------------------
        Microsoft::WRL::ComPtr<ID3D12Device> mpDevice{};                                       // �f�o�C�X
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue{};                           // �R�}���h�L���[
        Microsoft::WRL::ComPtr<IDXGISwapChain3> mpSwapChain{};                                 // �X���b�v�`�F�[��
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mpCommandAllocator[Orca::FrameCount]{}; // �R�}���h�A���P�[�^�[
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mpCommandList{};                     // �R�}���h���X�g
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpHeapRTV{};                              // �f�B�X�N���v�^�q�[�v�̃����_�[�^�[�Q�b�g
        Microsoft::WRL::ComPtr<ID3D12Resource> mpColorBuffer[Orca::FrameCount]{};              // �J���[�o�b�t�@
        D3D12_CPU_DESCRIPTOR_HANDLE mHandleRTV[Orca::FrameCount]{};                            // �����_�[�^�[�Q�b�g�r���[
        Microsoft::WRL::ComPtr<ID3D12Fence> mpFence{};                                         // �t�F���X�I�u�W�F�N�g


    private:
        void Present(uint32_t Interval_);   // ��ʂɕ\�����A���̃t���[���̏���������
        void WaitGpu();                     // GPU�̎��s��҂�
        void AddDebugFlag() const;                // �f�o�b�O�t���O��ǉ�����

        HANDLE mFenceEvent{}; 
        UINT mFrameIndex{};
        unsigned long long mFenceCounter[Orca::FrameCount]{};

    private:
        struct Vertex
        {
            DirectX::XMFLOAT3 Position{};
            DirectX::XMFLOAT2 TexCoord{};
        };

        struct alignas(256)CB_Simple
        {
            DirectX::XMMATRIX World{};
            DirectX::XMMATRIX ViewMat{};
            DirectX::XMMATRIX ProjMat{};
        };


        bool CreateDescriptorPool();
        void CreateViewport(); // �r���[�|�[�g���쐬����
        void CreateScissor(); // �V�U�[��`���쐬����


        Microsoft::WRL::ComPtr<ID3D12Resource> mpDepthBuffer{};
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpHeapDSV{};
        D3D12_CPU_DESCRIPTOR_HANDLE mHandleDsV{};

        DescriptorPool* mpPool[POOL_COUNT];         // �f�B�X�N���v�^�v�[���ł�.

    private:
        float mRotateAngle{};
        D3D12_RECT mScissor{};
        D3D12_VIEWPORT mViewPort{};
    };
}
