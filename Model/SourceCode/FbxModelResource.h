#pragma once
#include"ModelResource.h"
#include"Matrix.h"

#include"fbxsdk.h"

namespace Model
{
	struct FbxModelResource final :ModelResource
	{
		FbxModelResource() = default;
		~FbxModelResource() override = default;

        // �ǂݍ���
		void Load(const char* FileName_, const char* IgnoreRootMotionNodeName_ = nullptr);

		// �G�N�X�|�[�g
		void Export(const char* FileName_);
		// �A�j���[�V�����̖��O�������o��
		void ExportAnimationText(const char* Filename_) const;

		// �A�j���[�V�����ǉ��ǂݍ���
		void AddAnimation(const char* FileName_);

	private:
		// �m�[�h�f�[�^���\�z
		void LoadNodes(FbxNode* pFbxNode_, int ParentNodeIndex_);
		void LoadNode(FbxNode* pFbxNode_, int ParentNodeIndex_);

		// ���b�V���f�[�^��ǂݍ���
		void LoadMeshes(FbxNode* pFbxNode_);
		void LoadMesh(FbxNode* pFbxNode_, FbxMesh* pFbxMesh_);

		// �}�e���A���f�[�^��ǂݍ���
		void LoadMaterials(const char* DirName_, FbxScene* pFbxScene);
		void LoadMaterial(const char* DirName_, FbxSurfaceMaterial* fbxSurfaceMaterial);

		// �A�j���[�V�����f�[�^��ǂݍ���
		void LoadAnimations(FbxScene* pFbxScene, const char* name, bool append);

		// �E����W�n���獶����W�n�֕ϊ�
		void ConvertAxisSystemFromRHtoLH();

		// �m�[�h�p�X�擾
		std::string GetNodePath(FbxNode* pFbxNode_) const;

		// �m�[�hID�擾
		NodeId GetNodeId(FbxNode* pFbxNode_);

		// �}�e���A���C���f�b�N�X���擾����
		int FindMaterialIndex(FbxScene* pFbxScene, const FbxSurfaceMaterial* pFbxSurfaceMaterial_);

		// �������郂�[�V�����m�[�h
		void SetupIgnoreMotionNode(const char* IgnoreRootMotionNodeName_);

		// �ړ��l���E����W�n���獶����W�n�֕ϊ�����
		void ConvertTranslationFromRHtoLH(Math::Vector3 & translate);

		// ��]�l���E����W�n���獶����W�n�֕ϊ�����
		void ConvertRotationFromRHtoLH(Math::Quaternion& rotate);

		// �s��l���E����W�n���獶����W�n�֕ϊ�����
		void ConvertMatrixFromRHtoLH(Math::Matrix& matrix);

		// �C���f�b�N�X�o�b�t�@���E����W�n���獶����W�n�֕ϊ�����
		void ConvertIndexBufferFromRHtoLH(std::vector<UINT>& indices);

	private:
		int						mRootMotionNodeIndex = -1;
	};
}
