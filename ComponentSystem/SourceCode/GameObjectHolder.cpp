#include "pch.h"
#include"GameObjectHolder.h"

#include"ImGuiHelper.h"
void ComponentSystem::GameObjectHolder::AddGameObject(const std::string& Name_)
{
    // ---------------------------- �Q�[���I�u�W�F�N�g��ǉ�����֐� ---------------------------
    
}

void ComponentSystem::GameObjectHolder::Update(float Dt_)
{

}

void ComponentSystem::GameObjectHolder::GuiMenu()
{
    OrcaGui::MainMenu("System", "GameObjects", &mOpenGui);
}
