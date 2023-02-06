#pragma once
#include<cstdint>
#include<memory>
#include<string>

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
       explicit Component(uint32_t UpdatePriority_ = 0);
       virtual ~Component() = default;


       Component(const Component& RHS_) = delete;
       Component(Component&& LHS_) = delete;
       Component operator=(const Component& RHS_) = delete;
       Component& operator=(Component&& LHS_) = delete;

       virtual void OnStart() {}      // 更新の直前に一回だけ呼ばれる

       virtual void Update(float Dt_) {}
       virtual void GuiMenu(float Dt_){}

       // ------------------------------- セッター関数 -------------------------------
       void SetGameObject(const std::shared_ptr<ComponentSystem::GameObject>& pGameObject_);

       bool GetIsAlive()const;

   protected:
       const uint32_t mUpdatePriority; // 更新優先度（数字が大きいほど更新優先度が高い）
       std::weak_ptr<ComponentSystem::GameObject> mpGameObject{};
   };
}