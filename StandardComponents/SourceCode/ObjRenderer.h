#pragma once
#include"Renderer.h"
#include"Matrix.h"
// ---------------------------- ObjMesh��`�悷�邽�߂̃����_���[ ---------------------------

namespace Component
{
    class Transform;
    class ObjMesh;

    class ObjRenderer :public Renderer
    {
        // ------------------------------- �萔�o�b�t�@ ------------------------------
        struct alignas(256)Cb_Obj
        {
            Math::Matrix World{};
        };
    public:
        Cb_Obj* mCbData{};
    public:
        ObjRenderer();
        ~ObjRenderer() override = default;

        void OnStart() override;
        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) override;
    private:
        // ------------------------- �K�v�ȃR���|�[�l���g���L���b�V������ ------------------------
        std::weak_ptr<ObjMesh> mpObjMesh{};
        std::weak_ptr<Transform> mpTransform{};
    };

}
