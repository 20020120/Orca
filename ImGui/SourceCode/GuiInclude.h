#pragma once

// ImGui�Ɋւ���w�b�_���C���N���[�h����t�@�C��
#include"../ImGui/imgui.h"
#include"ImGuiHelper.h"
#include"ScopedGui.h"


// ------------------------------------ �}�N�� ------------------------------------
#define ORCA_GUI_TREE(Name,source)\
    if(ImGui::TreeNode((const char*)(Name)))\
    {\
    source\
    ImGui::TreePop();\
    }