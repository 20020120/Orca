#pragma once
#include"Component.h"
#include"FbxModelResource.h"
#include<tuple>

namespace OrcaGraphics
{
    namespace Resource
    {
        class ConstantBuffer;
    }
}
struct ID3D12GraphicsCommandList;
namespace Component
{
    class Transform;

    class FbxMesh final :public Component
    {
        friend class FbxRenderer;
    public:
        explicit FbxMesh(const char* FileName_);
        ~FbxMesh()override;
        
        FbxMesh() = delete;

        void OnStart() override;
        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;

        void StackGraphicsCmd(ID3D12GraphicsCommandList* pCmdList_);

        [[nodiscard]] uint32_t GetDescriptorIndex()const;
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
        std::vector<Node>	    mNodes{};
        std::vector<std::tuple<std::string, Node*>> mNodeNames{};
        Node* mpSelectedNode{};
    private:
        // ----------------------------- Guiの表示メニュー ----------------------------
        void GuiMenu_Nodes();
        void GuiMenu_Node(const Node* pNode_);
        void GuiMenu_Materials(std::vector<Model::FbxModelResource::Material>& Materials_) const;
        void GuiMenu_Animations(const std::vector<Model::FbxModelResource::Animation>& Animations_) const;

        // 位置を更新する
        void UpdateTransform();

    private:
        // キャッシュするコンポ―ンネント
        std::weak_ptr<Transform> mpTransform{};

        // ----------------------------- 描画用の定数バッファ ----------------------------
        static constexpr uint32_t MaxBones = 128;
        struct alignas(256) CbMesh
        {
            Math::Matrix mBoneTransforms[MaxBones];
        };
        std::unique_ptr<OrcaGraphics::Resource::ConstantBuffer> mpCb{};
    public:
        CbMesh* mpCbData{};
    };
}