#pragma once
#include"Component.h"
#include"FbxModelResource.h"
#include<tuple>
namespace Component
{
    class FbxMesh final :public Component
    {
    public:
        explicit FbxMesh(const char* FileName_);
        ~FbxMesh()override;
        
        FbxMesh() = delete;
        void GuiMenu(float Dt_) override;

        // ------------------------------- 描画リソース ------------------------------
        struct Node
        {
            std::string mName;
            Node* mpParent;
            Math::Vector3	mScale;
            Math::Quaternion	mRotate;
            Math::Vector3	mTranslate;
            Math::Matrix	mLocalTransform;
            Math::Matrix	mWorldTransform;

            std::vector<Node*>	mChildren;
        };
    private:
        Model::FbxModelResource mResource{};
        std::vector<Node>	    mNodes;
        std::vector<std::tuple<std::string, Node*>> mNodeNames{};
        Node* mpSelectedNode{};
        // ----------------------------- Guiの表示メニュー ----------------------------
    private:
        void GuiMenu_Nodes();
        void GuiMenu_Node(const Node* pNode_);
        void GuiMenu_Materials(std::vector<Model::FbxModelResource::Material>& Materials_) const;
        void GuiMenu_Animations(const std::vector<Model::FbxModelResource::Animation>& Animations_);

    };
}