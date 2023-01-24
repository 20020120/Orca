#include"ImGuiSetting.h"



#include"DescriptorHandle.h"
#include"DescriptorPool.h"

#include"../ImGui/backends/imgui_impl_dx12.h"
#include"../ImGui/backends/imgui_impl_win32.h"
#include"../ImGui/imgui.h"
#include<d3d12.h>

OrcaGraphics::DescriptorPool* ImGuiSetting::Renderer::mpDescriptorPool = {};
OrcaGraphics::DescriptorHandle* ImGuiSetting::Renderer::mpDescriptorHandle = {};

void ImGuiSetting::Renderer::CreateImGui(HWND Hwnd_, ID3D12Device* pDevice_,
    OrcaGraphics::DescriptorPool* pDescriptorPool_)
{
    mpDescriptorPool = pDescriptorPool_;
    mpDescriptorHandle = mpDescriptorPool->AllocHandle();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    //ImGui::GetIO().Fonts->AddFontFromFileTTF(
    //    path_japanese.c_str(), 18.0f, nullptr,
    //    glyphRangesJapanese);
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //ImGuiStyle& style = ImGui::GetStyle();
    //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //{
    //    style.WindowRounding = 0.0f;
    //    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    //}

    IOSettings(Hwnd_);


    
    const auto pDescriptorHeap_ = mpDescriptorPool->GetHeap();

    ImGui_ImplWin32_Init(Hwnd_);
    ImGui_ImplDX12_Init(pDevice_, 3,
        DXGI_FORMAT_R8G8B8A8_UNORM, pDescriptorHeap_,
        mpDescriptorHandle->HandleCPU,
        mpDescriptorHandle->HandleGPU
    );
    ImGui::StyleColorsClassic();
}

void ImGuiSetting::Renderer::IOSettings(HWND Hwnd_)
{
    // ImGuiドッキングのフラグ
    ImGuiIO& imgui_io = ImGui::GetIO();

    // Setup back-end capabilities flags
    imgui_io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    imgui_io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
    imgui_io.BackendPlatformName = "ImGuiRenderer";
    imgui_io.ImeWindowHandle = Hwnd_;

    imgui_io.KeyMap[ImGuiKey_Tab] = VK_TAB;
    imgui_io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
    imgui_io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
    imgui_io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
    imgui_io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
    imgui_io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
    imgui_io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
    imgui_io.KeyMap[ImGuiKey_Home] = VK_HOME;
    imgui_io.KeyMap[ImGuiKey_End] = VK_END;
    imgui_io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
    imgui_io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
    imgui_io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
    imgui_io.KeyMap[ImGuiKey_Space] = VK_SPACE;
    imgui_io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
    imgui_io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
    imgui_io.KeyMap[ImGuiKey_A] = 'A';
    imgui_io.KeyMap[ImGuiKey_C] = 'C';
    imgui_io.KeyMap[ImGuiKey_V] = 'V';
    imgui_io.KeyMap[ImGuiKey_X] = 'X';
    imgui_io.KeyMap[ImGuiKey_Y] = 'Y';
    imgui_io.KeyMap[ImGuiKey_Z] = 'Z';
}

void ImGuiSetting::Renderer::NewFrame()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiSetting::Renderer::Render()
{
    ImGui::Render();
}

void ImGuiSetting::Renderer::RenderDrawData(ID3D12GraphicsCommandList* pCmdList_)
{
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), pCmdList_);
}

void ImGuiSetting::Renderer::Cleanup()
{
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    mpDescriptorPool->FreeHandle(mpDescriptorHandle);
}
