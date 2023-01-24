#pragma once
#include"../ImGui/imgui.h"


// -------------------------------- ImGuiのヘルパー関数 -------------------------------
namespace OrcaGui
{
    // ---------------------------- メインメニューに要素を追加する ----------------------------
    void MainMenu(const char* Label_, const char* Detail_, bool* IsOpen_);

}
