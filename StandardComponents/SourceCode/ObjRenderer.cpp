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

void Component::ObjRenderer::Start()
{
    Renderer::Start();
    // ------------------------------- リソース情報を生成 -------------------------------
    OrcaGraphics::Dx12ResourceHolder::Add(mShaderType, mpGameObject.lock()->GetName());
}

void Component::ObjRenderer::Update(float Dt_)
{
    // ---------------------------- コンポーネントをキャッシュする ----------------------------
    if (!mCacheComponents)
    {
        mCacheComponents = true;
        if (mpGameObject.expired())
            return;
        mpTransform = mpGameObject.lock()->GetComponent<Transform>();
        mpObjMesh = mpGameObject.lock()->GetComponent<ObjMesh>();
    }
}

void Component::ObjRenderer::GuiMenu(float Dt_)
{
    if (ImGui::TreeNode("ObjRenderer"))
    {
        ImGui::RadioButton("Cached_Transform", !mpTransform.expired());
        ImGui::RadioButton("Cached_ObjMesh", !mpObjMesh.expired());
        ImGui::TreePop();
    }
}

void Component::ObjRenderer::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    if(mpTransform.expired()||mpObjMesh.expired())
        return;

    const auto& resource = mpObjMesh.lock()->GetResource();
    resource.mCbData->World = mpTransform.lock()->mTransform.Transpose();
    resource.StackGraphicsCmd(pCmdList_);
}
