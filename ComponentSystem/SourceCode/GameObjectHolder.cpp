#include "pch.h"
#include"GameObjectHolder.h"
#include"GuiInclude.h"
void ComponentSystem::GameObjectHolder::AddGameObject(const std::string& Name_)
{
    // ---------------------------- �Q�[���I�u�W�F�N�g��ǉ�����֐� ---------------------------
    mHolder.emplace_back(std::make_shared<GameObject>(Name_));
}

void ComponentSystem::GameObjectHolder::Update(float Dt_)
{

}

void ComponentSystem::GameObjectHolder::GuiMenu(float Dt_)
{
    const char* GuiName = "GameObjects";

    OrcaGui::MainMenu("System", GuiName, &mOpenGui);
    // -------------------------------------------------------------------------
    if (mOpenGui)
    {
        // -------------------------------- �ꗗ�\�� -------------------------------
        {
            OrcaGui::ScopedWindow window(GuiName);
            for (const auto& obj : mHolder)
            {
                if (ImGui::Button(obj->GetName().c_str()))
                {
                    mpGuiObject = obj;
                }
            }
            ImGui::Separator();
            if (ImGui::Button("Close"))
                mOpenGui = false;
        }
        // ------------------------- �ʂ�GameObject��Gui -------------------------
        if(mpGuiObject)
        {
            OrcaGui::ScopedWindow window(mpGuiObject->GetName().c_str());
            mpGuiObject->GuiMenu(Dt_);
            ImGui::Separator();
            if (ImGui::Button("Close"))
                mpGuiObject.reset();
        }
    }
}
