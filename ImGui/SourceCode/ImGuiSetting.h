#pragma once
#include<Windows.h>
// ------------------------------------ 前方宣言 -----------------------------------
struct ID3D12Device;
struct ID3D12DescriptorHeap;
struct ID3D12GraphicsCommandList;

namespace OrcaGraphics
{
    class DescriptorPool;
    class DescriptorHandle;
}


// -------------------------------- ImGuiの初期化・設定 -------------------------------
namespace ImGuiSetting
{
    // ------------------------------- ImGui描画クラス ------------------------------
    class Renderer
    {
    public:
        static void CreateImGui(HWND Hwnd_, ID3D12Device* pDevice_, OrcaGraphics::DescriptorPool* pDescriptorPool_);
        static void IOSettings(HWND Hwnd_);
        static void NewFrame();// 毎フレーム呼び出す関数
        static void Render();// 描画
        static void RenderDrawData(ID3D12GraphicsCommandList* pCmdList_);
        static void Cleanup();// 終了

    private:
        static OrcaGraphics::DescriptorPool* mpDescriptorPool;
        static OrcaGraphics::DescriptorHandle* mpDescriptorHandle;
    };
}
