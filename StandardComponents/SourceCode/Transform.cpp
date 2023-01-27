#include"Transform.h"

#include "GameObject.h"
#include"GuiInclude.h"

Component::Transform::Transform()
    :Component(1)
{}

Component::Transform::Transform(const Math::Vector3& Position_, const Math::Vector3& Scale_,
    const Math::Quaternion& Orientation_)
    :Component(1)
    , mPosition(Position_)
    , mScale(Scale_)
    , mOrientation(Orientation_)
{}

Component::Transform::~Transform()
{}

void Component::Transform::Update(float Dt_)
{
    // 親オブジェクトの存在を確認する
    GetParentTransform();
    
    mTransform = Math::Matrix::CreateWorld(mPosition, mScale, mOrientation);
    if(mIsGlobal)
    {
        return;
    }

    // ----------------------------- ローカル空間で計算するとき -----------------------------
    if(!mpParentTransform)  // 親がいない時終了
        return;

    // 親子関係を適用
    auto parentTransform = mpParentTransform->mTransform;
    mTransform = parentTransform * mTransform;
}

void Component::Transform::GuiMenu(float Dt_)
{
    if(ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat3("Position", &mPosition.x);
        ImGui::DragFloat3("Scale", &mScale.x);
        ImGui::DragFloat4("Orientation", &mOrientation.x);

        ImGui::Checkbox("IsGlobal", &mIsGlobal);
        ImGui::SameLine();
        ImGui::RadioButton("HasParent", mpParentTransform != nullptr);
        ImGui::TreePop();
    }
}

void Component::Transform::GetParentTransform()
{
    // ------------------------------ 最初の一回しか判定しない -----------------------------
    if (!mCheckHasParent)
    {
        mCheckHasParent = true;
        if (mpGameObject.expired())
            return;
        if (const auto parent = mpGameObject.lock()->GetParent())
        {
            mpParentTransform = parent->GetComponent<Transform>();
        }
        mCheckHasParent = true;
    }
}

