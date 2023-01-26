#pragma once
#include<cstdint>
#include<memory>

namespace ComponentSystem
{
    class GameObject;
}

// ------------------------------- �R���|�[�l���g�̊��N���X -------------------------------
namespace Component
{
   class Component
   {
   public:
       Component(uint32_t UpdatePriority_);
       virtual ~Component() = default;


       Component(const Component& RHS_) = delete;
       Component(Component&& LHS_) = delete;
       Component operator=(const Component& RHS_) = delete;
       Component& operator=(Component&& LHS_) = delete;

       virtual void Update(float Dt_) {}
       virtual void GuiMenu(float Dt_){}

       // ------------------------------- �Z�b�^�[�֐� -------------------------------
       void SetGameObject(const std::shared_ptr<ComponentSystem::GameObject>& pGameObject_);

   protected:
       uint32_t mUpdatePriority{}; // �X�V�D��x�i�������傫���قǍX�V�D��x�������j
       std::shared_ptr<ComponentSystem::GameObject> mpGameObject{};
   };
}