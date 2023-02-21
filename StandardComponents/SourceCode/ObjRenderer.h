#pragma once
#include"Renderer.h"
// ---------------------------- ObjMeshを描画するためのレンダラー ---------------------------
namespace OrcaGraphics
{
    namespace Resource
    {
        class ConstantBuffer;
    }
}

namespace Component
{
    class Transform;
    class ObjMesh;

    class ObjRenderer :public Renderer
    {
        // ------------------------------- 定数バッファ ------------------------------
        struct alignas(256) ResourceIndex
        {
            int CameraCBufferIndex;
            int ObjectCBufferIndex;
        };
      
    public:
        ObjRenderer();
        ~ObjRenderer() override = default;

        void OnStart() override;
        void GuiMenu(float Dt_) override;
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) override;
    private:
        // ------------------------------- 定数バッファ ------------------------------
        std::unique_ptr<OrcaGraphics::Resource::ConstantBuffer> mpCBuffer{};
        ResourceIndex* mpCbData{};
        // ------------------------- 必要なコンポーネントをキャッシュする ------------------------
        std::weak_ptr<ObjMesh> mpObjMesh{};
    };

}
