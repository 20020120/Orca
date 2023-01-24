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
        OrcaGui::ScopedWindow window(GuiName);

        for (const auto& obj : mHolder)
        {
            obj->GuiMenu(Dt_);
        }
    }
}
