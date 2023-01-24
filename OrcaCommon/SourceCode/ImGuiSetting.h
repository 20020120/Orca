#pragma once
#include<Windows.h>
// ------------------------------------ 前方宣言 -----------------------------------
struct ID3D12Device;
struct ID3D12DescriptorHeap;
struct ID3D12GraphicsCommandList;


// -------------------------------- ImGuiの初期化・設定 -------------------------------
namespace ImGuiSetting
{
    // ------------------------------- ImGuiを初期化 -------------------------------
    void CreateImGui(HWND Hwnd_, ID3D12Device* pDevice_, ID3D12DescriptorHeap* pDescriptorHeap_);

    void IOSettings(HWND Hwnd_);

    // 毎フレーム呼び出す関数
    void NewFrame();
    // 描画
    void Render();
    void RenderDrawData(ID3D12GraphicsCommandList* pCmdList_);
    // 終了
    void Cleanup();

}
