#pragma once

// ImGuiに関するヘッダをインクルードするファイル
#include"../ImGui/imgui.h"
#include"ImGuiHelper.h"
#include"ScopedGui.h"


// ------------------------------------ マクロ ------------------------------------
#define ORCA_GUI_TREE(Name,source)\
    if(ImGui::TreeNode((const char*)(Name)))\
    {\
    source\
    ImGui::TreePop();\
    }