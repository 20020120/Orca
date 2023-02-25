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
    public:
        FbxRenderer();
        ~FbxRenderer() override;
        void OnStart() override;
        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;
    private:
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) override;
        std::weak_ptr<FbxMesh> mpFbxMesh{};
    private:
        // ���\�[�X�̃C���f�b�N�X���Ǘ�����\����
        struct ResourceIndex
        {
            uint32_t mCamera{};
            uint32_t mNode{};
        };
        ResourceIndex mResourceIndex{};
    };
}