#include"pch.h"
#include"GameObject.h"
#include"GuiInclude.h"

#include<typeinfo>
ComponentSystem::GameObject::GameObject(const std::string& Name_)
{
    mName = Name_;
}

std::shared_ptr<ComponentSystem::GameObject> ComponentSystem::GameObject::AddChildObject(const std::string& Name_)
{
    auto gameObject = std::make_shared<GameObject>(Name_);
    gameObject->mpParent = shared_from_this();
    mChildObjects.emplace_back(gameObject);
    return gameObject;
}

std::shared_ptr<ComponentSystem::GameObject> ComponentSystem::GameObject::GetParent() const
{
    if (mpParent.expired())
        return nullptr;
    return mpParent.lock();
}

void ComponentSystem::GameObject::Start() const
{
    for(const auto& com: mComponents)
    {
        com->Start();
    }
    for (const auto& child : mChildObjects)
    {
        child->Start();
    }
}

void ComponentSystem::GameObject::Update(float Dt_) const
{
    for (const auto& com : mComponents)
    {
        com->Update(Dt_);
    }
    for (const auto& child : mChildObjects)
    {
        child->Update(Dt_);
    }
}

void ComponentSystem::GameObject::Delete()
{
    mIsAlive = false;
}

void ComponentSystem::GameObject::GuiMenu(float Dt_)
{
    ImGui::Text("Components");
    for (const auto& com : mComponents)
    {
        com->GuiMenu(Dt_);
    }
}

void ComponentSystem::GameObject::HolderGuiMenu(std::shared_ptr<GameObject>& pGuiObject_, float x, float y, int NestCounts_)
{
    ++NestCounts_;
    for (const auto& child : mChildObjects)
    {
        ImGui::Dummy({ NestCounts_ * 10.0f,0.0f });
        if(ImGui::Button(child->GetName().c_str()))
        {
            if (pGuiObject_ != child)
            {
                pGuiObject_ = child;
                ImGui::SetNextWindowPos({ x,y });
            }
            child->HolderGuiMenu(pGuiObject_, x, y, NestCounts_);
        }
    }
}


std::string ComponentSystem::GameObject::GetName() const
{
    return mName;
}

bool ComponentSystem::GameObject::GetIsAlive() const
{
    return mIsAlive;
}
