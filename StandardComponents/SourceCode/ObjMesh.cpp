#include"ObjMesh.h"
#include "GameObject.h"
#include"Obj.h"
#include"Transform.h"
#include"ConstantBuffer.h"
#include"GuiInclude.h"
#include <utility>

// ---------------------------------- �R���X�g���N�^ ----------------------------------
Component::ObjMesh::ObjMesh(const std::wstring& FilePath_)
    :Component(0)
{
    mFilePath = move(FilePath_);
    mObjResource.Initialize(mFilePath.c_str());
    // ------------------------------- �萔�o�b�t�@�������� ------------------------------
    mpConstantBuffer = std::make_unique<Graphics::Resource::ConstantBuffer>(&mpCbData);
}
// ----------------------------------- �f�X�g���N�^ ----------------------------------
Component::ObjMesh::~ObjMesh() = default;

// ------------------------------------ ������ ------------------------------------

void Component::ObjMesh::OnStart()
{
    // ----------------------------- �R���|�[�l���g���L���b�V�� -----------------------------
    mpTransform = mpGameObject.lock()->GetComponent<Transform>();
}

uint32_t Component::ObjMesh::GetDescriptorIndex() const
{
    return mpConstantBuffer->GetDescriptorIndex();
}

// ------------------------------------- �X�V ------------------------------------
void Component::ObjMesh::Update(float Dt_)
{
    mpCbData->World = mpTransform.lock()->mTransform.Transpose();
}

// ---------------------------------- ���\�[�X���擾 ----------------------------------
Model::Obj& Component::ObjMesh::GetResource() 
{
    return mObjResource;
}

// --------------------------------- ImGui�̃��j���[ --------------------------------
void Component::ObjMesh::GuiMenu(float Dt_)
{
    if (ImGui::TreeNode("Mesh"))
    {
        ImGui::TreePop();
    }
}
