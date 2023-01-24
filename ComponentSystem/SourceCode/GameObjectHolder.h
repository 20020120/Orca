#pragma once

#include<memory>
#include<vector>
#include<string>
namespace ComponentSystem
{
    // ---------------------------------- 前方宣言 ---------------------------------
    class GameObject;


    class GameObjectHolder
    {
    public:
        GameObjectHolder(uint32_t MaxIndex_);
        ~GameObjectHolder() = default;

        GameObjectHolder(const GameObjectHolder& RHS_) = delete;
        GameObjectHolder& operator =(const GameObjectHolder& RHS_) = delete;
        GameObjectHolder(GameObjectHolder&& LHS_)noexcept = delete;
        GameObjectHolder& operator =(GameObjectHolder&& LHS_) = delete;

        void Update(float Dt_); // 更新 

        void  AddGameObject(const std::string& Name_);


    private:
        std::vector<std::shared_ptr<GameObject>> mHolder{};
    };
}
