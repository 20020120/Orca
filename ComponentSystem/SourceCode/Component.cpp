#include "pch.h"
#include"Component.h"

Component::Component::Component(uint32_t UpdatePriority_)
    :mUpdatePriority(UpdatePriority_)
{}

void Component::Component::SetGameObject(const std::shared_ptr<ComponentSystem::GameObject>& pGameObject_)
{
    mpGameObject = pGameObject_;
}
