#pragma once
#include"Renderer.h"
#include"Matrix.h"
// ---------------------------- ObjMeshを描画するためのレンダラー ---------------------------

namespace Component
{
    class Transform;
    class ObjMesh;

    class ObjRenderer :public Renderer
    {
        // ------------------------------- 定数バッファ ------------------------------
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
        // ------------------------- 必要なコンポーネントをキャッシュする ------------------------
        std::weak_ptr<ObjMesh> mpObjMesh{};
        std::weak_ptr<Transform> mpTransform{};
    };

}
