#include"ObjRenderer.h"

#include"GameObject.h"
#include"Transform.h"
#include"ObjMesh.h"

#include"RendererSystem.h"
#include"Dx12ResourceHolder.h"
#include"ConstantBuffer.h"
#include"GuiInclude.h"

Component::ObjRenderer::ObjRenderer()
    :Renderer(OrcaGraphics::Shader::ShaderType::Sample)
{}

void Component::ObjRenderer::OnStart()
{
    Renderer::OnStart();
    mpObjMesh = mpGameObject.lock()->GetComponent<ObjMesh>();
    // ----------------------------- リソース情報をマッピングする ----------------------------
    mpCbData.ObjectCBufferIndex = mpObjMesh.lock()->GetDescriptorIndex();
    mpCbData.CameraCBufferIndex = mpObjMesh.lock()->GetDescriptorIndex();
}

void Component::ObjRenderer::GuiMenu(float Dt_)
{
    if (ImGui::TreeNode("ObjRenderer"))
    {
        ImGui::RadioButton("Cached_ObjMesh", !mpObjMesh.expired());
        ImGui::Text("ResourceHandle:%d", mResourceHandle);
        ImGui::TreePop();
    }
}

void Component::ObjRenderer::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    if(mpObjMesh.expired())
        return;
    // ---------------------------- 描画に必要なデータを取得 ---------------------------
    const auto& resource = mpObjMesh.lock()->GetResource();
    pCmdList_->SetGraphicsRoot32BitConstants(0, 2, reinterpret_cast<void*>(&mpCbData), 0);
    resource.StackGraphicsCmd(pCmdList_);
}
