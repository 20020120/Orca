#pragma once

#include<memory>
#include<vector>
#include<string>

#include "GameObject.h"

namespace ComponentSystem
{
    // ---------------------------------- �O���錾 ---------------------------------
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

        // ------------------------------- �w���p�[�֐� ------------------------------
        void AddGameObject(const std::string& Name_);   // �Q�[���I�u�W�F�N�g��ǉ�


        // ----------------------------- �P��I�ɌĂ΂��֐� ----------------------------
        void Update(float Dt_); // �X�V
        void GuiMenu(); // ImGui�̃��j���[��\��

    private:
        std::vector<std::shared_ptr<GameObject>> mHolder{};
        bool mOpenGui{};
    };
}
