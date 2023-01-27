#include"pch.h"
#include"GameObject.h"

#include"GuiInclude.h"
ComponentSystem::GameObject::GameObject(const std::string& Name_)
{
    mName = Name_;
}

void ComponentSystem::GameObject::GuiMenu(float Dt_)
{
    for (const auto& com : mComponents)
    {
        com->GuiMenu(Dt_);
    }
}

std::string ComponentSystem::GameObject::GetName() const
{
    return mName;
}
