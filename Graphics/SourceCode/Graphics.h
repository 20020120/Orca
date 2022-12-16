#pragma once
#include<d3d12.h>
#include<wrl.h>


namespace OrcaGraphics
{
    // DirectX12�̕`��N���X
    class Graphics
    {
    public:
        Graphics();
        ~Graphics();

        void Initialize();  // ������


    private:
        void CreateDevice(); // �f�o�C�X�̏�����

    private:
        // ----------------------------------- �ϐ� ----------------------------------
        Microsoft::WRL::ComPtr<ID3D12Device> mpDevice{}; // �f�o�C�X
    };
}