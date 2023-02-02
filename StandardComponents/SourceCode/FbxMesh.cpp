#include"FbxMesh.h"

#include"GuiInclude.h"
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
}

Component::FbxMesh::~FbxMesh()
{
}

void Component::FbxMesh::GuiMenu(float Dt_)
{
	ORCA_GUI_TREE("FbxMesh",
		GuiMenu_Nodes();
       )
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
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

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

void Component::FbxMesh::GuiMenu_Materials(const std::vector<Model::FbxModelResource::Material>& Materials_)
{
}

void Component::FbxMesh::GuiMenu_Vertices(const std::vector<Model::FbxModelResource::Vertex>& Vertices_)
{
}

void Component::FbxMesh::GuiMenu_Animations(const std::vector<Model::FbxModelResource::Animation>& Animations_)
{

}