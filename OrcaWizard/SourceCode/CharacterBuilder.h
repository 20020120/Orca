#pragma once
#include<memory>
#include <string>
// ------------------------------------ �O���錾 -----------------------------------
namespace ComponentSystem
{
    class GameObject;
}

namespace OrcaWizard
{
    void CharacterBuilder(std::shared_ptr<ComponentSystem::GameObject> pGameObject_, std::string FilePath_);
}
