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
        std::shared_ptr<GameObject> AddGameObject(const std::string& Name_);   // ゲームオブジェクトを追加

        // ----------------------------- 恒常的に呼ばれる関数 ----------------------------
        void Update(float Dt_); // 更新
        void GuiMenu(float Dt_); // ImGuiのメニューを表示

        void Finalize();    // 終了処理
    private:
        std::vector<std::shared_ptr<GameObject>> mHolder{};
        std::vector<std::shared_ptr<GameObject>> mInitializer{}; // 初期化用コンテナ

        // ------------------------------ オプション変数 ------------------------------
        bool mOpenGui{};    //Guiを開いているかどうか
        std::shared_ptr<GameObject> mpGuiObject{};  // 選択中のGameObjectをGuiに出す
    };
}