#pragma once
#include "Component.h"

#include<memory>
#include<vector>
#include<string>
#include<functional>


namespace Component
{ // ---------------------------------- 前方宣言 ---------------------------------
    class Component;
}

namespace ComponentSystem
{

    class GameObject final :public std::enable_shared_from_this<GameObject>
    {
    public:
        explicit GameObject(const std::string& Name_);
        ~GameObject() = default;

        template<class Type, class... T>
        void AddComponent(T&&... Arg_);

        template<class T>
        std::shared_ptr<T> GetComponent()const;
        std::shared_ptr<GameObject> AddChildObject(const std::string& Name_);   // ゲームオブジェクトを追加
        std::shared_ptr<GameObject> GetParent()const;

        void Update(float Dt_);
        void Delete();
        
        void GuiMenu(float Dt_);
        void HolderGuiMenu(std::shared_ptr<GameObject>& pGuiObject_, float x, float y, int NestCounts_ = 0);

        // -------------------------------- ゲッター -------------------------------
        std::string GetName()const;
    private:
        std::string mName{};    // 名前
        std::vector <std::shared_ptr<Component::Component>> mComponents{}; // 所持しているコンポ―ンネント
        std::vector<std::shared_ptr<GameObject>> mChildObjects{};   // 子オブジェクト
        std::weak_ptr<GameObject> mpParent{};

        // ------------------------------ オプション変数 ------------------------------
        bool mIsAlive{ true };  // 生存判定
        bool mOpenGui{};
        std::shared_ptr<Component::Component> mpGuiComponent{};
    };
}

// ------------------------------- コンポーネントを追加する関数 ------------------------------
template <class Type,class ... T>
void ComponentSystem::GameObject::AddComponent(T&&... Arg_)
{
    const std::shared_ptr<Component::Component> component = std::make_shared<Type>(std::forward<T>(Arg_)...);
    component->SetGameObject(shared_from_this());
    mComponents.emplace_back(component);
}

// ------------------------------- コンポーネントを取得する関数 ------------------------------
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
