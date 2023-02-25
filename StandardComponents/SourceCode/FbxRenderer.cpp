#include"FbxRenderer.h"

#include "Dx12ResourceHolder.h"
#include "GameObject.h"
#include"Transform.h"
#include"FbxMesh.h"
#include"RendererSystem.h"
#include"Graphics.h"
#include"GuiInclude.h"
#include"Camera.h"
Component::FbxRenderer::FbxRenderer()
    :Renderer(OrcaGraphics::Shader::ShaderType::Lambert)
{}

Component::FbxRenderer::~FbxRenderer() = default;


void Component::FbxRenderer::OnStart()
{
	// ---------------------------- コンポーネントをキャッシュする ----------------------------
	if (mpGameObject.expired())
		return;
	mpFbxMesh = mpGameObject.lock()->GetComponent<FbxMesh>();

    // ---------------------------------- リソースID ----------------------------------
	mResourceIndex.mNode = mpFbxMesh.lock()->GetDescriptorIndex();
	mResourceIndex.mCamera = OrcaGraphics::Camera::Instance().GetDescriptorIndex();

	Renderer::OnStart();
}

void Component::FbxRenderer::Update(float Dt_)
{

}

void Component::FbxRenderer::GuiMenu(float Dt_)
{
    if(ImGui::TreeNode("FbxRenderer"))
    {
        ImGui::RadioButton("Cached_FbxMesh", !mpFbxMesh.expired());
        ImGui::Text("ResourceHandle:%d", mResourceHandle);
        ImGui::TreePop();
    }
}


void Component::FbxRenderer::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    // ---------------------------- リソースを取得していなければ関数終了 ----------------------------
	if(mpFbxMesh.expired())
		return;

    pCmdList_->SetGraphicsRoot32BitConstants(0, 2, &mResourceIndex, 0);
    // --------------------------------- Fbxの描画 --------------------------------
	const auto fbxMesh = mpFbxMesh.lock();
    fbxMesh->StackGraphicsCmd(pCmdList_.Get());
}