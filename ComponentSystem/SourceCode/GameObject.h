#pragma once
#include<memory>
#include<vector>
#include<string>
namespace ComponentSystem
{
    // ---------------------------------- �O���錾 ---------------------------------
    class Component;

    class GameObject final 
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
        std::vector <std::shared_ptr<Component>> mComponents{}; // �������Ă���R���|�\���l���g

        // ------------------------------ �I�v�V�����ϐ� ------------------------------
        bool mIsAlive{ true };  // ��������
        bool mOpenGui{};
    };
}

// ------------------------------- �R���|�[�l���g��ǉ�����֐� ------------------------------
template <class ... T>
void ComponentSystem::GameObject::AddComponent(T&&... Arg_)
{
    mComponents.emplace_back(std::make_shared<T>(std::forward<T>(Arg_)...));
}

// ------------------------------- �R���|�[�l���g���擾����֐� ------------------------------
template <class T>
std::shared_ptr<T> ComponentSystem::GameObject::GetComponent() const
{
    for (auto& ptr : mComponents)
    {
        auto com = std::dynamic_pointer_cast<T>(ptr);
        if (com)
            return com;
    }
    return nullptr;
}
