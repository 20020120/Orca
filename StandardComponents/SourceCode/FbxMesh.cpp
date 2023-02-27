#include "FbxMesh.h"
#include "GameObject.h"
#include"Transform.h"
#include "GuiInclude.h"
#include"ConstantBuffer.h"
#include"WinFileDialog.h"
Component::FbxMesh::FbxMesh(const char* FileName_)
{
	mResource.Load(FileName_);
    // ------------------------------------ ノード -----------------------------------
	const std::vector<Resource::ModelResource::Node>& resNodes = mResource.GetNodes();
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
    // ----------------------------------- メッシュ -----------------------------------
	// メッシュ
	const std::vector<Resource::ModelResource::Mesh>& resMesh = mResource.GetMeshes();
	mMeshes.resize(resMesh.size());
	for (auto i = 0; i < resMesh.size(); ++i)
	{
		auto&& src = resMesh.at(i);
		auto&& mesh = mMeshes.at(i);
		mesh.mpCb = std::make_unique<Graphics::Resource::ConstantBuffer>(&mesh.mpCbData);
		mesh.mpMesh = &src;
	}

	// ----------------------------- 描画用のDx12のリソースを作成 -----------------------------
	for (auto& mesh : mResource.mMeshes)
	{
		mesh.mVertexBuffer.Create(mesh.mVertices);
		mesh.mIndexBuffer.Create(mesh.mIndices);
	}
	
}

Component::FbxMesh::~FbxMesh()
{}

void Component::FbxMesh::OnStart()
{
	// コンポーネントをキャッシュ
	mpTransform = mpGameObject.lock()->GetComponent<Transform>();
	UpdateTransform();
}

void Component::FbxMesh::Update(float Dt_)
{
	// ----------------------------------- 更新関数 -----------------------------------
	UpdateTransform();
	UpdateAnimation(Dt_);
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
	if(ImGui::TreeNode("Animation"))
	{
		if (ImGui::TreeNode("LoadAnimation"))
		{

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Player"))
		{

			int i = 0;
			for (auto& anim : mResource.mAnimations)
			{
				if (ImGui::Button(anim.mName.c_str()))
				{
					PlayAnimation(i);
				}
				++i;
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

}

void Component::FbxMesh::EditorGuiMenu(float Dt_)
{
	ImGui::Begin("Transform");
	mpTransform.lock()->GuiMenu(Dt_);
	ImGui::End();

	ImGui::Begin("Materials");

	ImGui::End();

	ImGui::Begin("Export");
	if (ImGui::Button("Export"))
	{
		std::string fileName{};
		if (WinApi::FileDialog::GetFolderNameA(fileName))
		{
			fileName += "\\aa.sss";
			mResource.Export(fileName.c_str());
		}
	}
	ImGui::End();
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

void Component::FbxMesh::GuiMenu_Materials(std::vector<Resource::FbxModelResource::Material>& Materials_) const
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

void Component::FbxMesh::GuiMenu_Animations(const std::vector<Resource::FbxModelResource::Animation>& Animations_) const
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
	for (auto& node : mNodes)
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

	for (auto& mesh : mMeshes)
	{
		const auto* res_mesh = mesh.mpMesh;
		if (res_mesh->mNodeIndices.size() > 0)
		{
			for (size_t i = 0; i < res_mesh->mNodeIndices.size(); ++i)
			{
				auto w = mNodes.at(res_mesh->mNodeIndices.at(i)).mWorldTransform;
				auto o = res_mesh->mOffsetTransforms.at(i);
				auto res = o * w;
			    mesh.mpCbData->mBoneTransforms[i] = res.Transpose();
			}
		}
		else
		{
			mesh.mpCbData->mBoneTransforms[0] = mNodes.at(res_mesh->mNodeIndex).mWorldTransform.Transpose();
		}
	}
}

void Component::FbxMesh::UpdateAnimation(float Dt_)
{
	if (mCurrentAnimation < 0||mResource.GetAnimations().empty())
	{
		return;
	}

	const auto& animation = mResource.GetAnimations().at(mCurrentAnimation);

	const auto& keyframes = animation.mKeyframes;
	const int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// 現在の時間がどのキーフレームの間にいるか判定する
		const auto& keyframe0 = keyframes.at(keyIndex);
		const auto& keyframe1 = keyframes.at(keyIndex + 1);
		if (mCurrentSeconds >= keyframe0.mSeconds && mCurrentSeconds < keyframe1.mSeconds)
		{
            const float rate = (mCurrentSeconds - keyframe0.mSeconds) / (keyframe1.mSeconds - keyframe0.mSeconds);
			assert(mNodes.size() == keyframe0.mNodeKeys.size());
			assert(mNodes.size() == keyframe1.mNodeKeys.size());
            const int nodeCount = static_cast<int>(mNodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// ２つのキーフレーム間の補完計算
				const auto& key0 = keyframe0.mNodeKeys.at(nodeIndex);
				const auto& key1 = keyframe1.mNodeKeys.at(nodeIndex);

				Node& node = mNodes[nodeIndex];
				node.mScale = Math::Vector3::Lerp(key0.mScale, key1.mScale, rate);
				node.mRotate = Math::Quaternion::Lerp(key0.mRotate, key1.mRotate, rate);
				node.mTranslate = Math::Vector3::Lerp(key0.mTranslate, key1.mTranslate, rate);
			}
			break;
		}
	}

	// 最終フレーム処理
	if (mEndAnimation)
	{
		mEndAnimation = false;
		mCurrentAnimation = -1;
		return;
	}

	// 時間経過
	mCurrentSeconds += Dt_;
	if (mCurrentSeconds >= animation.mSecondsLength)
	{
		if (mLoopAnimation)
		{
			mCurrentSeconds -= animation.mSecondsLength;
		}
		else
		{
			mCurrentSeconds = animation.mSecondsLength;
			mEndAnimation = true;
		}
	}
}

bool Component::FbxMesh::IsPlayAnimation() const
{
	return mCurrentAnimation != -1;
}

void Component::FbxMesh::PlayAnimation(const int Index_, const bool IsLoop)
{
	mCurrentAnimation = Index_;
	mLoopAnimation = IsLoop;
	mEndAnimation = false;
	mCurrentSeconds = 0.0f;
}

void Component::FbxMesh::StopAnimation()
{
	mCurrentAnimation = -1;
}
