#include"pch.h"
#include"GameObject.h"

#include"GuiInclude.h"

ComponentSystem::GameObject::GameObject(const std::string& Name_)
{
    mName = Name_;
}

void ComponentSystem::GameObject::GuiMenu(float Dt_)
{
    if (ImGui::TreeNode(mName.c_str()))
    {
        ImGui::TreePop();
    }
}
