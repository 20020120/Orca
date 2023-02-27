#include"FbxRenderer.h"

#include "GameObject.h"
#include"Transform.h"
#include"FbxMesh.h"
#include"RendererSystem.h"
#include"Graphics.h"
#include"GuiInclude.h"
#include"Camera.h"
Component::FbxRenderer::FbxRenderer()
    :Renderer(Graphics::Shader::ShaderType::Lambert)
{}

Component::FbxRenderer::~FbxRenderer() = default;


void Component::FbxRenderer::OnStart()
{
	// ---------------------------- コンポーネントをキャッシュする ----------------------------
	if (mpGameObject.expired())
		return;
	mpFbxMesh = mpGameObject.lock()->GetComponent<FbxMesh>();

    // ---------------------------------- リソースID ----------------------------------
	mResourceIndex.mCamera = Graphics::Camera::Instance().GetDescriptorIndex();

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

	const auto& fbxResource = mpFbxMesh.lock()->mMeshes;
	for (auto& mesh : fbxResource)
	{
		mResourceIndex.mNode = mesh.mpCb->GetDescriptorIndex();
		pCmdList_->SetGraphicsRoot32BitConstants(0, 2, &mResourceIndex, 0);
		pCmdList_->IASetVertexBuffers(0, 1, &mesh.mpMesh->mVertexBuffer.mVbView);
		pCmdList_->IASetIndexBuffer(&mesh.mpMesh->mIndexBuffer.mIbView);
		pCmdList_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const auto& subset : mesh.mpMesh->mSubsets)
		{
			pCmdList_->DrawIndexedInstanced(subset.mIndexCount, 1, subset.mStartIndex, 0, 0);
		}
	}
}