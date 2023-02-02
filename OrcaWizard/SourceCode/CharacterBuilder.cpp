#include "CharacterBuilder.h"

#include"GameObjectHolder.h"
#include"Transform.h"
#include"FbxMesh.h"

void OrcaWizard::CharacterBuilder(ComponentSystem::GameObjectHolder& Holder_)
{
    // -------------------------- �G�f�B�^�ŕ\������L�����N�^�[���������� -------------------------
    const auto gameObject = Holder_.AddGameObject("Character");
    gameObject->AddComponent<Component::Transform>();
    gameObject->AddComponent<Component::FbxMesh>("../Resource/Model/ww.fbx");
}
