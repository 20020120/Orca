#pragma once
#include "Component.h"

#include<memory>
#include<vector>
#include<string>


namespace Component
{ // ---------------------------------- �O���錾 ---------------------------------
    class Component;
}

namespace ComponentSystem
{

    class GameObject final :public std::enable_shared_from_this<GameObject>
    {
    public:
        explicit GameObject(const std::string& Name_);

        template<class... T>
        void AddComponent(T&&... Arg_);

        template<class T>
        std::shared_ptr<T> GetComponent()const;

        void Delete();
        void GuiMenu(float Dt_);

        // -------------------------------- �Q�b�^�[ -------------------------------
        std::string GetName()const; 
    private:
        std::string mName{};    // ���O
        std::vector <std::shared_ptr<Component::Component>> mComponents{}; // �������Ă���R���|�\���l���g

        // ------------------------------ �I�v�V�����ϐ� ------------------------------
        bool mIsAlive{ true };  // ��������
        bool mOpenGui{};
    };
}

// ------------------------------- �R���|�[�l���g��ǉ�����֐� ------------------------------
template <class ... T>
void ComponentSystem::GameObject::AddComponent(T&&... Arg_)
{
    const std::shared_ptr<Component::Component> component = std::make_shared<T>(std::forward<T>(Arg_)...);
    component->SetGameObject(shared_from_this());
    mComponents.emplace_back();
}

// ------------------------------- �R���|�[�l���g���擾����֐� ------------------------------
template <class T>
std::shared_ptr<T> ComponentSystem::GameObject::GetComponent() const
{
    for (auto& ptr : mComponents)
    {
        if (auto com = std::dynamic_pointer_cast<T>(ptr))
            return com;
    }
    return nullptr;
}
