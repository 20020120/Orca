#include"ObjMesh.h"
#include "GameObject.h"
#include"Obj.h"
#include"Transform.h"
#include"ConstantBuffer.h"
#include"GuiInclude.h"
#include <utility>

// ---------------------------------- コンストラクタ ----------------------------------
Component::ObjMesh::ObjMesh(const std::wstring& FilePath_)
    :Component(0)
{
    mFilePath = move(FilePath_);
    mObjResource.Initialize(mFilePath.c_str());
    // ------------------------------- 定数バッファを初期化 ------------------------------
    mpConstantBuffer = std::make_unique<Graphics::Resource::ConstantBuffer>(&mpCbData);
}
// ----------------------------------- デストラクタ ----------------------------------
Component::ObjMesh::~ObjMesh() = default;

// ------------------------------------ 初期化 ------------------------------------

void Component::ObjMesh::OnStart()
{
    // ----------------------------- コンポーネントをキャッシュ -----------------------------
    mpTransform = mpGameObject.lock()->GetComponent<Transform>();
}

uint32_t Component::ObjMesh::GetDescriptorIndex() const
{
    return mpConstantBuffer->GetDescriptorIndex();
}

// ------------------------------------- 更新 ------------------------------------
void Component::ObjMesh::Update(float Dt_)
{
    mpCbData->World = mpTransform.lock()->mTransform.Transpose();
}

// ---------------------------------- リソースを取得 ----------------------------------
Model::Obj& Component::ObjMesh::GetResource() 
{
    return mObjResource;
}

// --------------------------------- ImGuiのメニュー --------------------------------
void Component::ObjMesh::GuiMenu(float Dt_)
{
    if (ImGui::TreeNode("Mesh"))
    {
        ImGui::TreePop();
    }
}
