#pragma once
#include"Component.h"
#include"Vector3.h"
#include"Matrix.h"
#include"ModelResource.h"

#include<vector>
namespace Graphics
{
    namespace Resource
    {
        class ConstantBuffer;
    }
}

namespace Component
{
    class Transform;;

    /**
     * \brief モデルクラス
     */
    class Model final :public Component
    {
    public:
        Model(const char* FileName_);


        void OnStart() override;
        void Update(float Dt_) override;
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
        // ----------------------------- 描画用の定数バッファ ----------------------------
        static constexpr uint32_t MaxBones = 128;
        struct alignas(256) CbMesh
        {
            Math::Matrix mBoneTransforms[MaxBones];
        };
        struct Mesh
        {
            const Resource::ModelResource::Mesh * mpMesh{};
            std::unique_ptr<Graphics::Resource::ConstantBuffer> mpCb{};
            CbMesh* mpCbData{};
        };
    private:
        Resource::ModelResource mResource{};
        std::vector<Node>	    mNodes{};
        std::vector<Mesh>	    mMeshes{};
        std::vector<std::tuple<std::string, Node*>> mNodeNames{};
        Node* mpSelectedNode{};
    private:
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
