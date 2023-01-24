#pragma once

#include<memory>
#include<vector>
#include<string>
namespace ComponentSystem
{
    // ---------------------------------- ëOï˚êÈåæ ---------------------------------
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

        void Update(float Dt_); // çXêV 

        void  AddGameObject(const std::string& Name_);


    private:
        std::vector<std::shared_ptr<GameObject>> mHolder{};
    };
}
