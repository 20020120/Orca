#pragma once
#include <cstdint>
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>
#include"ScreenConstants.h"
namespace OrcaGraphics
{
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

        void Present(uint32_t Interval_);   // ��ʂɕ\�����A���̃t���[���̏���������

    private:
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

        HANDLE mFenceEvent{}; 
        UINT mFrameIndex{};
        unsigned long long mFenceCounter[Orca::FrameCount]{};
    };
}