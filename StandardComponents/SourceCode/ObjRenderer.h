#pragma once
#include"Renderer.h"

// ---------------------------- ObjMesh��`�悷�邽�߂̃����_���[ ---------------------------

namespace Component
{
    class Transform;
    class ObjMesh;

    class ObjRenderer :public Renderer
    {
    public:
        ObjRenderer();
        ~ObjRenderer() override = default;

        void Start() override;
        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) override;
    private:
        // ------------------------- �K�v�ȃR���|�[�l���g���L���b�V������ ------------------------
        std::weak_ptr<ObjMesh> mpObjMesh{};
        std::weak_ptr<Transform> mpTransform{};
        // ------------------------------ �I�v�V�����ϐ� ------------------------------
        bool mCacheComponents{ false };
    };

}
