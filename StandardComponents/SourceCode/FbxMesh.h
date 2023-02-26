#pragma once
#include"Component.h"
#include"FbxModelResource.h"
#include<tuple>
#include"ConstantBuffer.h"

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
        void EditorGuiMenu(float Dt_);
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
        // ----------------------------- 描画用の定数バッファ ----------------------------
        static constexpr uint32_t MaxBones = 128;
        struct alignas(256) CbMesh
        {
            Math::Matrix mBoneTransforms[MaxBones];
        };
        struct Mesh
        {
            const Model::ModelResource::Mesh* mpMesh{};
            std::unique_ptr<OrcaGraphics::Resource::ConstantBuffer> mpCb{};
            CbMesh* mpCbData{};
        };
    private:
        Model::FbxModelResource mResource{};
        std::vector<Node>	    mNodes{};
        std::vector<Mesh>	    mMeshes{};
        std::vector<std::tuple<std::string, Node*>> mNodeNames{};
        Node* mpSelectedNode{};
    private:
        // ----------------------------- Guiの表示メニュー ----------------------------
        void GuiMenu_Nodes();
        void GuiMenu_Node(const Node* pNode_);
        void GuiMenu_Materials(std::vector<Model::FbxModelResource::Material>& Materials_) const;
        void GuiMenu_Animations(const std::vector<Model::FbxModelResource::Animation>& Animations_) const;

        
        void UpdateTransform(); // 位置を更新する

        // --------------------------- アニメーションに関する機能 ---------------------------
    private:
        int								mCurrentAnimation = -1;
        float							mCurrentSeconds = 0.0f;
        bool							mLoopAnimation = false;
        bool							mEndAnimation = false;

        void UpdateAnimation(float Dt_);    // アニメーションを更新する
        bool IsPlayAnimation()const;
        void PlayAnimation(int Index_, bool IsLoop = false);
        void StopAnimation();
    private:
        // キャッシュするコンポ―ンネント
        std::weak_ptr<Transform> mpTransform{};
    };
}