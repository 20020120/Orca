#pragma once
#include<Windows.h>
// ------------------------------------ �O���錾 -----------------------------------
struct ID3D12Device;
struct ID3D12DescriptorHeap;
struct ID3D12GraphicsCommandList;

namespace OrcaGraphics
{
    class DescriptorPool;
    class DescriptorHandle;
}


// -------------------------------- ImGui�̏������E�ݒ� -------------------------------
namespace ImGuiSetting
{
    // ------------------------------- ImGui�`��N���X ------------------------------
    class Renderer
    {
    public:
        static void CreateImGui(HWND Hwnd_, ID3D12Device* pDevice_, OrcaGraphics::DescriptorPool* pDescriptorPool_);
        static void IOSettings(HWND Hwnd_);
        static void NewFrame();// ���t���[���Ăяo���֐�
        static void Render();// �`��
        static void RenderDrawData(ID3D12GraphicsCommandList* pCmdList_);
        static void Cleanup();// �I��

    private:
        static OrcaGraphics::DescriptorPool* mpDescriptorPool;
        static OrcaGraphics::DescriptorHandle* mpDescriptorHandle;
    };
}
