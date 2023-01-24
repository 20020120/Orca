#include "pch.h"
#include"GameObjectHolder.h"
#include"GuiInclude.h"
void ComponentSystem::GameObjectHolder::AddGameObject(const std::string& Name_)
{
    // ---------------------------- ゲームオブジェクトを追加する関数 ---------------------------
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
        // -------------------------------- 一覧表示 -------------------------------
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
        // ------------------------- 個別のGameObjectのGui -------------------------
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
