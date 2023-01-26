#include"Transform.h"
#include"GuiInclude.h"
void Component::Transform::Update(float Dt_)
{
    mTransform = Math::Matrix::CreateWorld(mPosition, mScale, mOrientation);
    if(mIsGlobal)
    {
        return;
    }

    // ----------------------------- ���[�J����ԂŌv�Z����Ƃ� -----------------------------
    if(!mpParentTransform)  // �e�����Ȃ����I��
        return;

    // �e�q�֌W��K�p
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
