#pragma once
#include<memory>
#include<vector>
#include<string>
namespace ComponentSystem
{
    // ---------------------------------- 前方宣言 ---------------------------------
    class Component;

    class GameObject final 
    {
    public:
        GameObject(const std::string& Name_);

        template<class... T>
        void AddComponent(T&&... Arg_);

        template<class T>
        std::shared_ptr<T> GetComponent()const;

        void Delete();  // 削除
    private:
        std::string mName{};    // 名前
        std::vector <std::shared_ptr<Component>> mComponents{}; // 所持しているコンポ―ンネント

        // ------------------------------ オプション変数 ------------------------------
        bool mIsAlive{ true };  // 生存判定
    };
}

// ------------------------------- コンポーネントを追加する関数 ------------------------------
template <class ... T>
void ComponentSystem::GameObject::AddComponent(T&&... Arg_)
{
    mComponents.emplace_back(std::make_shared<T>(std::forward<T>(Arg_)...));
}

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
