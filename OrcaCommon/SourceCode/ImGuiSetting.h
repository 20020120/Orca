#pragma once
#include<Windows.h>
// ------------------------------------ �O���錾 -----------------------------------
struct ID3D12Device;
struct ID3D12DescriptorHeap;
struct ID3D12GraphicsCommandList;


// -------------------------------- ImGui�̏������E�ݒ� -------------------------------
namespace ImGuiSetting
{
    // ------------------------------- ImGui�������� -------------------------------
    void CreateImGui(HWND Hwnd_, ID3D12Device* pDevice_, ID3D12DescriptorHeap* pDescriptorHeap_);

    void IOSettings(HWND Hwnd_);

    // ���t���[���Ăяo���֐�
    void NewFrame();
    // �`��
    void Render();
    void RenderDrawData(ID3D12GraphicsCommandList* pCmdList_);
    // �I��
    void Cleanup();

}
