#pragma once
#include<cstdint>
#include<memory>

namespace ComponentSystem
{
    class GameObject;
}

// ------------------------------- コンポーネントの基底クラス -------------------------------
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

       // ------------------------------- セッター関数 -------------------------------
       void SetGameObject(const std::shared_ptr<ComponentSystem::GameObject>& pGameObject_);

   protected:
       uint32_t mUpdatePriority{}; // 更新優先度（数字が大きいほど更新優先度が高い）
       std::shared_ptr<ComponentSystem::GameObject> mpGameObject{};
   };
}