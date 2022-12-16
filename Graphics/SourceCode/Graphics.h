#pragma once
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>

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

    private:
        void CreateDevice();        // �f�o�C�X�̏�����
        void CreateCommandQueue();  // �R�}���h�L���[�̏�����
        void CreateSwapChain(HWND hWnd_);     // �X���b�v�`�F�[���̍쐬
    private:
        // ----------------------------------- �ϐ� ----------------------------------
        Microsoft::WRL::ComPtr<ID3D12Device> mpDevice{};             // �f�o�C�X
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue{}; // �R�}���h�L���[
        Microsoft::WRL::ComPtr<IDXGISwapChain3> mpSwapChain{};       // �X���b�v�`�F�[��

        UINT mFrameIndex{};


    };
}