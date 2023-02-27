#pragma once
#include"GraphicsMacro.h"
#include"ScreenConstants.h"
#include"GraphicsMinimum.h"

#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>
#include<DirectXMath.h>

// ------------------------------------ �O���錾 -----------------------------------
namespace Graphics
{
    class DescriptorPool;
}
struct ID3D12CommandQueue;

// ---------------------------- Dx12�̃O���t�B�b�N�X�N���X�̎������� ---------------------------
namespace Graphics
{
    class GraphicsImpl
    {
    public:
        GraphicsImpl() = default;
        ~GraphicsImpl() = default;

        void Initialize(HWND hWnd_);  // ������
        void Finalize(); // �I������

        void OpenCmdList() const;
        void CloseCmdList();
        void StackCmdList() const;

        [[nodiscard]] OrcaComPtr(ID3D12Device) GetDevice()const;
        [[nodiscard]] OrcaComPtr(ID3D12GraphicsCommandList) GetCmdList()const;
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
        OrcaComPtr(ID3D12Device) mpDevice{};                                       // �f�o�C�X
        OrcaComPtr(ID3D12CommandQueue)  mpCommandQueue{};                           // �R�}���h�L���[
        OrcaComPtr(IDXGISwapChain3 ) mpSwapChain{};                                 // �X���b�v�`�F�[��
        OrcaComPtr(ID3D12CommandAllocator ) mpCommandAllocator[Orca::FrameCount]{}; // �R�}���h�A���P�[�^�[
        OrcaComPtr(ID3D12GraphicsCommandList ) mpCommandList{};                     // �R�}���h���X�g
        OrcaComPtr(ID3D12DescriptorHeap ) mpHeapRTV{};                              // �f�B�X�N���v�^�q�[�v�̃����_�[�^�[�Q�b�g
        OrcaComPtr(ID3D12Resource) mpColorBuffer[Orca::FrameCount]{};              // �J���[�o�b�t�@
        D3D12_CPU_DESCRIPTOR_HANDLE mHandleRTV[Orca::FrameCount]{};                            // �����_�[�^�[�Q�b�g�r���[
        OrcaComPtr(ID3D12Fence) mpFence {};                                         // �t�F���X�I�u�W�F�N�g


    private:
        void Present(uint32_t Interval_);   // ��ʂɕ\�����A���̃t���[���̏���������
        void WaitGpu();                     // GPU�̎��s��҂�
        void AddDebugFlag() const;                // �f�o�b�O�t���O��ǉ�����

        HANDLE mFenceEvent{};
        UINT mFrameIndex{};
        unsigned long long mFenceCounter[Orca::FrameCount]{};

    private:
        bool CreateDescriptorPool();
        void CreateViewport(); // �r���[�|�[�g���쐬����
        void CreateScissor(); // �V�U�[��`���쐬����

        Microsoft::WRL::ComPtr<ID3D12Resource> mpDepthBuffer{};
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpHeapDSV{};
        D3D12_CPU_DESCRIPTOR_HANDLE mHandleDsV{};

        DescriptorPool* mpPool[POOL_COUNT]{};         // �f�B�X�N���v�^�v�[���ł�.`
    private:
        float mRotateAngle{};
        D3D12_RECT mScissor{};
        D3D12_VIEWPORT mViewPort{};
    };
}
