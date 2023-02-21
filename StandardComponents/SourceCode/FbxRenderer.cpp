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
	mpTransform = mpGameObject.lock()->GetComponent<Transform>();
	mpFbxMesh = mpGameObject.lock()->GetComponent<FbxMesh>();
	Renderer::OnStart();
}

void Component::FbxRenderer::Update(float Dt_)
{
    // ----------------------------- 定数バッファのデータを更新 -----------------------------
	mCb->mWorld = mpTransform.lock()->mTransform;
}

void Component::FbxRenderer::GuiMenu(float Dt_)
{
    if(ImGui::TreeNode("FbxRenderer"))
    {
        ImGui::RadioButton("Cached_Transform", !mpTransform.expired());
        ImGui::RadioButton("Cached_FbxMesh", !mpFbxMesh.expired());
        ImGui::Text("ResourceHandle:%d", mResourceHandle);
        ImGui::TreePop();
    }
}


void Component::FbxRenderer::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    // ---------------------------- リソースを取得していなければ関数終了 ----------------------------
	if(mpFbxMesh.expired()||mpTransform.expired())
		return;
    // --------------------------------- Fbxの描画 --------------------------------
	const auto fbxMesh = mpFbxMesh.lock();
	const auto& resource = fbxMesh->mResource;
	auto& nodes = fbxMesh->mNodes;
    auto cmdList = OrcaGraphics::Graphics::GetCmdList();

	for (auto& mesh : resource.mMeshes)
	{
        // -------------------------------- 描画リソースを設定 --------------------------------
		mesh.StackGraphicsCmd(pCmdList_.Get());
		//OrcaGraphics::Camera::Instance().StackGraphicsCmd(pCmdList_);
		for (const auto& subSet : mesh.mSubsets)
		{
			// 描画
			pCmdList_->DrawIndexedInstanced(subSet.mIndexCount, 1, subSet.mStartIndex, 0, 0);
		}
	}
}