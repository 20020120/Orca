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
    // �e�I�u�W�F�N�g�̑��݂��m�F����
    GetParentTransform();
    
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

void Component::Transform::GetParentTransform()
{
    // ------------------------------ �ŏ��̈�񂵂����肵�Ȃ� -----------------------------
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

