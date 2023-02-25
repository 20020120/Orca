#include "FbxMesh.h"
#include "GameObject.h"
#include"Transform.h"
#include "GuiInclude.h"
#include"ConstantBuffer.h"
#include<d3d12.h>
Component::FbxMesh::FbxMesh(const char* FileName_)
{
	mResource.Load(FileName_);
	// ノード
	const std::vector<Model::ModelResource::Node>& resNodes = mResource.GetNodes();

	mNodes.resize(resNodes.size());
	mNodeNames.reserve(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < mNodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = mNodes.at(nodeIndex);

		dst.mName = src.mName.c_str();
		dst.mpParent = src.mParentIndex >= 0 ? &mNodes.at(src.mParentIndex) : nullptr;
		dst.mScale = src.mScale;
		dst.mRotate = src.mRotate;
		dst.mTranslate = src.mTranslate;

		if (dst.mpParent != nullptr)
		{
			dst.mpParent->mChildren.emplace_back(&dst);
		}
		mNodeNames.emplace_back(std::make_tuple(src.mName, &dst));
	}
	// ----------------------------- 描画用のDx12のリソースを作成 -----------------------------
	for (auto& mesh : mResource.mMeshes)
	{
		mesh.mVertexBuffer.Create(mesh.mVertices);
		mesh.mIndexBuffer.Create(mesh.mIndices);
	}
	// 定数バッファを作成
	mpCb = std::make_unique<OrcaGraphics::Resource::ConstantBuffer>(&mpCbData);
}

Component::FbxMesh::~FbxMesh()
{}

void Component::FbxMesh::OnStart()
{
	// コンポーネントをキャッシュ
	mpTransform = mpGameObject.lock()->GetComponent<Transform>();
}

void Component::FbxMesh::Update(float Dt_)
{
	// ----------------------------------- 更新関数 -----------------------------------
	UpdateTransform();
	
}

void Component::FbxMesh::GuiMenu(float Dt_)
{
	if (ImGui::TreeNode("FbxMesh"))
	{
		GuiMenu_Nodes();
		GuiMenu_Materials(mResource.mMaterials);
		GuiMenu_Animations(mResource.mAnimations);
		ImGui::TreePop();
	}
}

void Component::FbxMesh::StackGraphicsCmd(ID3D12GraphicsCommandList* pCmdList_)
{
	for (auto& mesh : mResource.mMeshes)
	{
		// メッシュ用定数バッファ更新
		for (auto& tra : mpCbData->mBoneTransforms)
		{
			tra.Clear();
		}

		if (mesh.mNodeIndices.size() > 0)
		{
			for (size_t i = 0; i < mesh.mNodeIndices.size(); ++i)
			{
				const auto& a = mNodes.at(mesh.mNodeIndices.at(i)).mWorldTransform;
				const auto& b = mesh.mOffsetTransforms.at(i);
				const auto boneTransform = b * a;
				mpCbData->mBoneTransforms[i] = boneTransform;
			}
		}
		else
		{
			mpCbData->mBoneTransforms[0] = mNodes.at(mesh.mNodeIndex).mWorldTransform;
		}
		pCmdList_->IASetVertexBuffers(0, 1, &mesh.mVertexBuffer.mVbView);
		pCmdList_->IASetIndexBuffer(&mesh.mIndexBuffer.mIbView);
		pCmdList_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const auto& subset : mesh.mSubsets)
		{
			pCmdList_->DrawIndexedInstanced(subset.mIndexCount, 1, subset.mStartIndex, 0, 0);
		}
	}
}

uint32_t Component::FbxMesh::GetDescriptorIndex() const
{
	return mpCb->GetDescriptorIndex();
}

void Component::FbxMesh::GuiMenu_Nodes()
{
#if 0
	decltype(mNodeNames) passedNodes{};
	if (ImGui::TreeNode("Search Node"))
	{
		static ImGuiTextFilter filter;
		filter.Draw();
		for (auto& tuple : mNodeNames)
		{
			if (filter.PassFilter(std::get<0>(tuple).c_str()))
			{
				passedNodes.emplace_back(tuple);
			}
		}
		for (auto& tuple : passedNodes)
		{
			GuiMenu_Node(std::get<1>(tuple));
		}
		ImGui::TreePop();
	}
#endif 
	for (auto& node : mNodes)
	{
		if (node.mpParent == nullptr)
		{
			GuiMenu_Node(&node);
		}
	}
}

void Component::FbxMesh::GuiMenu_Node(const Node* pNode_)
{
	// 矢印をクリック、またはノードをダブルクリックで階層を開く
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

	// 子がいない場合は矢印をつけない
    const size_t child_count = pNode_->mChildren.size();
	if (child_count <= 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	// ツリーノードを表示
    // 開かれている場合、子階層も同じ処理を行う
	if (const bool opened = ImGui::TreeNodeEx(pNode_, nodeFlags, pNode_->mName.c_str()); opened && child_count > 0)
	{
		for (const Node* child : pNode_->mChildren)
		{
			GuiMenu_Node(child);
		}
		ImGui::TreePop();
	}
}

void Component::FbxMesh::GuiMenu_Materials(std::vector<Model::FbxModelResource::Material>& Materials_) const
{
	// 矢印をクリック、またはノードをダブルクリックで階層を開く
    constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (ImGui::TreeNodeEx("Materials",nodeFlags))
	{
		for (auto& material : Materials_)
		{
			if (ImGui::TreeNode(material.mName.c_str()))
			{
				ImGui::Text(material.mTextureFileName.c_str());
				ImGui::DragFloat4("Color", &material.mColor.x);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void Component::FbxMesh::GuiMenu_Animations(const std::vector<Model::FbxModelResource::Animation>& Animations_) const
{
	// 矢印をクリック、またはノードをダブルクリックで階層を開く
	constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (ImGui::TreeNodeEx("Animation", nodeFlags))
	{
		for (const auto& anim : Animations_)
		{
			ImGui::Text(anim.mName.c_str());
		}
		ImGui::TreePop();
	}
}

void Component::FbxMesh::UpdateTransform()
{
 // ---------------------------------- 位置を更新する ---------------------------------
	auto transform = mpTransform.lock()->mTransform;
	for (Node& node : mNodes)
	{
		// ローカル行列算出
		auto S = Math::Matrix::ComputeScaleMatrix(node.mScale);
		auto R = Math::Matrix::ComputeRotationMatrix(node.mRotate);
		auto T = Math::Matrix::ComputeTranslateMatrix(node.mTranslate);
		auto LocalTransform = S * R * T;

		// ワールド行列算出
		Math::Matrix ParentTransform;
		if (node.mpParent != nullptr)
		{
			ParentTransform = node.mpParent->mWorldTransform;
		}
		else
		{
			ParentTransform = transform;
		}
		node.mLocalTransform = LocalTransform;
		node.mWorldTransform = LocalTransform * ParentTransform;
	}
}
