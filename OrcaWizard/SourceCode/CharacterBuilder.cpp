#include "CharacterBuilder.h"
#include"GameObject.h"
#include"Transform.h"
#include"FbxMesh.h"
#include"FbxRenderer.h"

void OrcaWizard::CharacterBuilder(std::shared_ptr<ComponentSystem::GameObject> pGameObject_,std::string FilePath_)
{
    pGameObject_->AddComponent<Component::Transform>();
    pGameObject_->AddComponent<Component::FbxMesh>(FilePath_.c_str());
    pGameObject_->AddComponent<Component::FbxRenderer>();
}
