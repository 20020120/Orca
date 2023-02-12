#pragma once
#include"Renderer.h"
#include"Matrix.h"
#include<memory>
namespace Component
{
    // ---------------------------------- �O���錾 ---------------------------------
    class FbxMesh;
    class Transform;

    class FbxRenderer :public Renderer
    {
        // ------------------------------- �萔�o�b�t�@ ------------------------------
        struct alignas(256) CbData
        {
            Math::Matrix mWorld{};
        };
    public:
        FbxRenderer();
        ~FbxRenderer() override;
        void OnStart() override;
        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;
    private:
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) override;
        std::weak_ptr<FbxMesh> mpFbxMesh{};
        std::weak_ptr<Transform> mpTransform{};
        CbData* mCb{};
    };
}