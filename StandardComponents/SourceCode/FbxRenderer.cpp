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
	// ---------------------------- �R���|�[�l���g���L���b�V������ ----------------------------
	if (mpGameObject.expired())
		return;
	mpTransform = mpGameObject.lock()->GetComponent<Transform>();
	mpFbxMesh = mpGameObject.lock()->GetComponent<FbxMesh>();
	Renderer::OnStart();
}

void Component::FbxRenderer::Update(float Dt_)
{
    // ----------------------------- �萔�o�b�t�@�̃f�[�^���X�V -----------------------------
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
    // ---------------------------- ���\�[�X���擾���Ă��Ȃ���Ί֐��I�� ----------------------------
	if(mpFbxMesh.expired()||mpTransform.expired())
		return;
    // --------------------------------- Fbx�̕`�� --------------------------------
	const auto fbxMesh = mpFbxMesh.lock();
	const auto& resource = fbxMesh->mResource;
	auto& nodes = fbxMesh->mNodes;
    auto cmdList = OrcaGraphics::Graphics::GetCmdList();

	for (auto& mesh : resource.mMeshes)
	{
        // -------------------------------- �`�惊�\�[�X��ݒ� --------------------------------
		mesh.StackGraphicsCmd(pCmdList_.Get());
		//OrcaGraphics::Camera::Instance().StackGraphicsCmd(pCmdList_);
		for (const auto& subSet : mesh.mSubsets)
		{
			// �`��
			pCmdList_->DrawIndexedInstanced(subSet.mIndexCount, 1, subSet.mStartIndex, 0, 0);
		}
	}
}