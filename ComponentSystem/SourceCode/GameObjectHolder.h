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
        std::shared_ptr<GameObject> AddGameObject(const std::string& Name_);   // �Q�[���I�u�W�F�N�g��ǉ�

        // ----------------------------- �P��I�ɌĂ΂��֐� ----------------------------
        void Update(float Dt_); // �X�V
        void GuiMenu(float Dt_); // ImGui�̃��j���[��\��

        void Finalize();    // �I������
    private:
        std::vector<std::shared_ptr<GameObject>> mHolder{};
        std::vector<std::shared_ptr<GameObject>> mInitializer{}; // �������p�R���e�i

        // ------------------------------ �I�v�V�����ϐ� ------------------------------
        bool mOpenGui{};    //Gui���J���Ă��邩�ǂ���
        std::shared_ptr<GameObject> mpGuiObject{};  // �I�𒆂�GameObject��Gui�ɏo��
    };
}