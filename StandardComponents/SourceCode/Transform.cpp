#include"Transform.h"
#include"GuiInclude.h"
void Component::Transform::Update(float Dt_)
{
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
