#include "pch.h"
#include"GameObjectHolder.h"
#include"GuiInclude.h"
std::shared_ptr<ComponentSystem::GameObject> ComponentSystem::GameObjectHolder::AddGameObject(const std::string& Name_)
{
    // ---------------------------- ゲームオブジェクトを追加する関数 ---------------------------
    auto gameObject = std::make_shared<GameObject>(Name_);
    mInitializer.emplace_back(gameObject);
    return gameObject;
}

void ComponentSystem::GameObjectHolder::Update(float Dt_)
{
    // ----------------------------- 最初のフレームで初期化する -----------------------------
    for(const auto& obj: mInitializer)
    {
        obj->OnStart();
        mHolder.emplace_back(obj);
    }
    mInitializer.clear();

    for (const auto& obj : mHolder)
    {
        obj->Update(Dt_);
    }
}

void ComponentSystem::GameObjectHolder::GuiMenu(float Dt_)
{
    const char* GuiName = "GameObjects";

    OrcaGui::MainMenu("System", GuiName, &mOpenGui);
    // -------------------------------------------------------------------------
    
    if (mOpenGui)
    {
        ImVec2 childWindowPos;
        // -------------------------------- 一覧表示 -------------------------------
        {
            OrcaGui::ScopedWindow window(GuiName);

            // ImGuiのウインドウの位置をいい感じの位置に設定する
            const auto windowWidth = ImGui::GetWindowWidth();
            const auto windowPos = ImGui::GetWindowPos();
            childWindowPos = { windowPos.x + windowWidth,windowPos.y };

            for (const auto& obj : mHolder)
            {
                if(ImGui::Button(obj->GetName().c_str()))
                {
                    if (mpGuiObject != obj)
                    {
                        mpGuiObject = obj;
                        ImGui::SetNextWindowPos(childWindowPos);
                    }
                }
                obj->HolderGuiMenu(mpGuiObject, childWindowPos.x, childWindowPos.y);
            }
            ImGui::Separator();
            if (ImGui::Button("Close"))
                mOpenGui = false;
        }
        // ------------------------- 個別のGameObjectのGui -------------------------
        if(mpGuiObject)
        {
            OrcaGui::ScopedWindow window(mpGuiObject->GetName().c_str());
            ImGui::SetWindowPos(childWindowPos);
            mpGuiObject->GuiMenu(Dt_);
            ImGui::Separator();
            if (ImGui::Button("Close"))
                mpGuiObject.reset();
        }
    }
}

void ComponentSystem::GameObjectHolder::Finalize()
{
    mpGuiObject.reset();
    mHolder.clear();
}
