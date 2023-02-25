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

Component::Transform::~Transform() = default;

void Component::Transform::Update(float Dt_)
{
    // �e�I�u�W�F�N�g�̑��݂��m�F����
    GetParentTransform();
    Math::Matrix parentMatrix{};
    mTransform = Math::Matrix::CreateWorld(mPosition, mScale, mOrientation);

    // ----------------------------- �e�̉e�����󂯂�Ƃ� -----------------------------
    if(!mIsGlobal&& mpParentTransform)
    {
        parentMatrix = mpParentTransform->mTransform;
    }
    // �e�q�֌W��K�p
    mTransform = parentMatrix * mTransform;
    // -------------------------------- ���W�n�𐮂��� --------------------------------
    ApplyCoordinate();

}

void Component::Transform::GuiMenu(float Dt_)
{
    if(ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat3("Position", &mPosition.x, 0.1f);
        ImGui::DragFloat3("Scale", &mScale.x);
        ImGui::SameLine();
        if (ImGui::Button("All"))
        {
            mScale.y = mScale.z = mScale.x;
        }

        ImGui::DragFloat4("Orientation", &mOrientation.x);

        ImGui::Checkbox("IsGlobal", &mIsGlobal);
        ImGui::SameLine();
        ImGui::RadioButton("HasParent", mpParentTransform != nullptr);
        ImGui::TreePop();
    }
}

void Component::Transform::SetCoordinate(Math::Coordinate Coord_)
{
    mCoordinate = Coord_;
}

void Component::Transform::ApplyCoordinate()
{
    constexpr DirectX::XMFLOAT4X4 coordTransform[]{
       { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	        // 0:RHS Y-UP
       { 1 , 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		    // 1:LHS Y-UP
       { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	    // 2:RHS Z-UP
       { 1 , 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		    // 3:LHS Z-UP
       { 1 , 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1 },		// 4:LHS -Y-UP
       { 1 , 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1 },		// 4:ReverseZ
       { 1 , 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 5:YMinusLHS
    };

    constexpr float scaleFactor = 1.0f;
    const auto C{ XMLoadFloat4x4(&coordTransform[static_cast<int>(mCoordinate)]) *
        DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) };
    XMStoreFloat4x4(&mTransform, C * XMLoadFloat4x4(&mTransform));
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

