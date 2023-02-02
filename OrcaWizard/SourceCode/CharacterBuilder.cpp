#include "CharacterBuilder.h"

#include"GameObjectHolder.h"
#include"Transform.h"
#include"FbxMesh.h"

void OrcaWizard::CharacterBuilder(ComponentSystem::GameObjectHolder& Holder_)
{
    // -------------------------- エディタで表示するキャラクターを準備する -------------------------
    const auto gameObject = Holder_.AddGameObject("Character");
    gameObject->AddComponent<Component::Transform>();
    gameObject->AddComponent<Component::FbxMesh>("../Resource/Model/ww.fbx");
}
