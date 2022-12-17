#pragma once
#include <cstdint>
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>
#include<DirectXMath.h>
#include"ScreenConstants.h"
namespace OrcaGraphics
{

    // ---------------------------------- �O���錾 ---------------------------------



    // DirectX12�̕`��N���X
    class Graphics
    {
    public:
        Graphics();
        ~Graphics();

        void Initialize(HWND hWnd_);  // ������
        void Finalize(); // �I������

        void Render();  // �`�揈��

    private:
        void CreateDevice();                  // �f�o�C�X�̏�����
        void CreateCommandQueue();            // �R�}���h�L���[�̏�����
        void CreateSwapChain(HWND hWnd_);     // �X���b�v�`�F�[���̍쐬
        void CreateCommandAllocator();        // �R�}���h�A���P�[�^�[���쐬
        void CreateCommandList();             // �R�}���h���X�g���쐬
        void CreateRenderTargetView();        // �����_�[�^�[�Q�b�g�r���[���쐬
        void CreateFence();                   // �t�F���X�I�u�W�F�N�g���쐬����

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
            DirectX::XMFLOAT4 Color{};
        };

        struct alignas(256)CB_Simple
        {
            DirectX::XMMATRIX World{};
            DirectX::XMMATRIX ViewMat{};
            DirectX::XMMATRIX ProjMat{};
        };
        template<class T>
        struct ConstantBufferView
        {
            D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc{};
            D3D12_CPU_DESCRIPTOR_HANDLE mHandleCPU{};
            D3D12_GPU_DESCRIPTOR_HANDLE mHandleGPU{};
            T* mpBuffer{};
        };

        void CreateVertexBuffer(); // ���_�o�b�t�@���쐬����
        void CreateConstantBuffer(); // �萔�o�b�t�@���쐬����

        Microsoft::WRL::ComPtr<ID3D12Resource> mpVertexBuffer{};
        D3D12_VERTEX_BUFFER_VIEW mVbView{};

        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpHeapCbV{};
        Microsoft::WRL::ComPtr<ID3D12Resource> mpConstantBuffer[Orca::FrameCount]{};
        ConstantBufferView<CB_Simple> mCbV[Orca::FrameCount]{};

        D3D12_VIEWPORT mViewPort{};
    };
}