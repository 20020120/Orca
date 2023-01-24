#pragma once

#include<memory>
#include<vector>
#include<string>

#include "GameObject.h"

namespace ComponentSystem
{
    // ---------------------------------- 前方宣言 ---------------------------------
    class GameObject;


    class GameObjectHolder
    {
    public:
        GameObjectHolder() = default;
        ~GameObjectHolder() = default;

        GameObjectHolder(const GameObjectHolder& RHS_) = delete;
        GameObjectHolder& operator =(const GameObjectHolder& RHS_) = delete;
        GameObjectHolder(GameObjectHolder&& LHS_)noexcept = delete;
        GameObjectHolder& operator =(GameObjectHolder&& LHS_) = delete;

        // ------------------------------- ヘルパー関数 ------------------------------
        void AddGameObject(const std::string& Name_);   // ゲームオブジェクトを追加


        // ----------------------------- 恒常的に呼ばれる関数 ----------------------------
        void Update(float Dt_); // 更新
        void GuiMenu(); // ImGuiのメニューを表示

    private:
        std::vector<std::shared_ptr<GameObject>> mHolder{};
        bool mOpenGui{};
    };
}
