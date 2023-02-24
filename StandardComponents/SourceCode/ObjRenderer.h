#pragma once
#include"Renderer.h"
// ---------------------------- ObjMesh��`�悷�邽�߂̃����_���[ ---------------------------
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
        // ------------------------------- �萔�o�b�t�@ ------------------------------
        struct ResourceIndex
        {
            uint32_t CameraCBufferIndex;
            uint32_t ObjectCBufferIndex;
        };
      
    public:
        ObjRenderer();
        ~ObjRenderer() override = default;

        void OnStart() override;
        void GuiMenu(float Dt_) override;
        void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_) override;
    private:
        // ------------------------------- �萔�o�b�t�@ ------------------------------
        ResourceIndex mpCbData{};
        // ------------------------- �K�v�ȃR���|�[�l���g���L���b�V������ ------------------------
        std::weak_ptr<ObjMesh> mpObjMesh{};
    };

}
