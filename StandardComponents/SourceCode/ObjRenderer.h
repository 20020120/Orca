#pragma once
#include"Renderer.h"

// ---------------------------- ObjMeshを描画するためのレンダラー ---------------------------

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
        // ------------------------- 必要なコンポーネントをキャッシュする ------------------------
        std::weak_ptr<ObjMesh> mpObjMesh{};
        std::weak_ptr<Transform> mpTransform{};
        // ------------------------------ オプション変数 ------------------------------
        bool mCacheComponents{ false };
    };

}
