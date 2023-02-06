#include"ObjRenderer.h"

#include"GameObject.h"
#include"Transform.h"
#include"ObjMesh.h"

#include"RendererSystem.h"
#include"Dx12ResourceHolder.h"
#include"GuiInclude.h"

Component::ObjRenderer::ObjRenderer()
    :Renderer(OrcaGraphics::Shader::ShaderType::Sample)
{}

void Component::ObjRenderer::OnStart()
{
    Renderer::OnStart();
    // ------------------------------- リソース情報を生成 -------------------------------
    mResourceHandle = OrcaGraphics::Dx12ResourceHolder::Add(mShaderType);
    mpTransform = mpGameObject.lock()->GetComponent<Transform>();
    mpObjMesh = mpGameObject.lock()->GetComponent<ObjMesh>();
    // ----------------------------- リソース情報をマッピングする ----------------------------
    OrcaGraphics::Dx12ResourceHolder::CbMapping(mResourceHandle, "Obj", &mCbData);
    OrcaGraphics::Dx12ResourceHolder::TexLoad(mResourceHandle, std::tuple("gDiffuseTexture", mpObjMesh.lock()->GetResource().GetTextureName()));
}

void Component::ObjRenderer::Update(float Dt_)
{
    mCbData->World = mpTransform.lock()->mTransform.Transpose();
}

void Component::ObjRenderer::GuiMenu(float Dt_)
{
    if (ImGui::TreeNode("ObjRenderer"))
    {
        ImGui::RadioButton("Cached_Transform", !mpTransform.expired());
        ImGui::RadioButton("Cached_ObjMesh", !mpObjMesh.expired());
        ImGui::Text("ResourceHandle:%d", mResourceHandle);
        ImGui::TreePop();
    }
}

void Component::ObjRenderer::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    if(mpTransform.expired()||mpObjMesh.expired())
        return;

    const auto& resource = mpObjMesh.lock()->GetResource();
    OrcaGraphics::Dx12ResourceHolder::StackGraphicsCmd(pCmdList_.Get(), mResourceHandle);
    resource.StackGraphicsCmd(pCmdList_);
}
