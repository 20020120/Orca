#include "pch.h"
#include "FbxModelResource.h"
#include"Encoding.h"
#include "Misc.h"

#include<d3d12.h>
#include<fstream>

// FbxDouble2 �� XMFLOAT2
inline Math::Vector2 FbxDouble2ToFloat2(const FbxDouble2& fbxValue)
{
	return Math::Vector2(
		static_cast<float>(fbxValue[0]),
		static_cast<float>(fbxValue[1])
	);
}

// FbxDouble3 �� XMFLOAT3
inline Math::Vector3 FbxDouble3ToFloat3(const FbxDouble3& fbxValue)
{
	return Math::Vector3(
		static_cast<float>(fbxValue[0]),
		static_cast<float>(fbxValue[1]),
		static_cast<float>(fbxValue[2])
	);
}

// FbxDouble4 �� XMFLOAT3
inline Math::Vector3 FbxDouble4ToFloat3(const FbxDouble4& fbxValue)
{
	return Math::Vector3(
		static_cast<float>(fbxValue[0]),
		static_cast<float>(fbxValue[1]),
		static_cast<float>(fbxValue[2])
	);
}

// FbxDouble4 �� XMFLOAT4
inline Math::Vector4 FbxDouble4ToFloat4(const FbxDouble4& fbxValue)
{
	return Math::Vector4(
		static_cast<float>(fbxValue[0]),
		static_cast<float>(fbxValue[1]),
		static_cast<float>(fbxValue[2]),
		static_cast<float>(fbxValue[3])
	);
}

// FbxDouble4 �� XMFLOAT4
inline Math::Matrix FbxAMatrixToFloat4x4(const FbxAMatrix& fbxValue)
{
	return Math::Matrix(
		static_cast<float>(fbxValue[0][0]),
		static_cast<float>(fbxValue[0][1]),
		static_cast<float>(fbxValue[0][2]),
		static_cast<float>(fbxValue[0][3]),
		static_cast<float>(fbxValue[1][0]),
		static_cast<float>(fbxValue[1][1]),
		static_cast<float>(fbxValue[1][2]),
		static_cast<float>(fbxValue[1][3]),
		static_cast<float>(fbxValue[2][0]),
		static_cast<float>(fbxValue[2][1]),
		static_cast<float>(fbxValue[2][2]),
		static_cast<float>(fbxValue[2][3]),
		static_cast<float>(fbxValue[3][0]),
		static_cast<float>(fbxValue[3][1]),
		static_cast<float>(fbxValue[3][2]),
		static_cast<float>(fbxValue[3][3])
	);
}


void Resource::FbxModelResource::Load(const char* FileName_, const char* IgnoreRootMotionNodeName_)
{
	// �f�B���N�g���p�X�擾
	char drive[32], dir[256], dirname[256];
	::_splitpath_s(FileName_, drive, sizeof(drive), dir, sizeof(dir), nullptr, 0, nullptr, 0);
	::_makepath_s(dirname, sizeof(dirname), drive, dir, nullptr, nullptr);

	const char* ext = strrchr(FileName_, '.');
	if (::_stricmp(ext, ".fbx") == 0)
	{
		// FBX�̃t�@�C���p�X��UTF-8�ɂ���K�v������
		char fbxFilename[256];
		Encoding::StringToUTF8(FileName_, fbxFilename, sizeof(fbxFilename));

		FbxManager* fbxManager = FbxManager::Create();

		// FBX�ɑ΂�����o�͂��`����
		FbxIOSettings* fbxIOS = FbxIOSettings::Create(fbxManager, IOSROOT);	// ���ʂȗ��R���Ȃ�����IOSROOT���w��
		fbxManager->SetIOSettings(fbxIOS);

		// �C���|�[�^�𐶐�
		FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
		bool result = fbxImporter->Initialize(fbxFilename, -1, fbxManager->GetIOSettings());	// -1�Ńt�@�C���t�H�[�}�b�g��������
		_ASSERT_EXPR_A(result, "FbxImporter::Initialize() : Failed!!\n");

		// Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
		FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
		fbxImporter->Import(fbxScene);
		fbxImporter->Destroy();	// �V�[���𗬂����񂾂�Importer�͉������OK

								// �W�I���g�����O�p�`�����Ă���
		FbxGeometryConverter fbxGeometryConverter(fbxManager);
		fbxGeometryConverter.Triangulate(fbxScene, true);
		fbxGeometryConverter.RemoveBadPolygonsFromMeshes(fbxScene);

		// ����Z-Up�̏ꍇ��Y-Up�ɕϊ�����
        if (const FbxAxisSystem fbx_scene_axis_system = fbxScene->GetGlobalSettings().GetAxisSystem(); fbx_scene_axis_system == FbxAxisSystem::eMayaZUp)
		{
			FbxAxisSystem::MayaYUp.ConvertScene(fbxScene);
		}

		// ���f���ǂݍ���
		std::vector<FbxNode*> fbxNodes;
		FbxNode* fbxRootNode = fbxScene->GetRootNode();
		LoadMaterials(dirname, fbxScene);
		LoadNodes(fbxRootNode, -1);
		LoadMeshes(fbxRootNode);

		// �������郂�[�V�����m�[�h������
		SetupIgnoreMotionNode(IgnoreRootMotionNodeName_);

		// �A�j���[�V�����ǂݍ���
		char name[256];
		::_splitpath_s(FileName_, nullptr, 0, nullptr, 0, name, 256, nullptr, 0);

		LoadAnimations(fbxScene, nullptr, false);

		// �}�l�[�W�����
		fbxManager->Destroy();		// �֘A���邷�ׂẴI�u�W�F�N�g����������
	}
	else
	{
		// �f�V���A���C�Y
		Deserialize(FileName_);

		// �������郂�[�V�����m�[�h������
		SetupIgnoreMotionNode(IgnoreRootMotionNodeName_);
	}

	// ���f���\�z
	//BuildModel(device, dirname);
}

void Resource::FbxModelResource::Export(const char* FileName_)
{
	Serialize(FileName_);
}

void Resource::FbxModelResource::AddAnimation(const char* FileName_)
{// FBX�̃t�@�C���p�X��UTF-8�ɂ���K�v������
	char fbxFilename[256];
	Encoding::StringToUTF8(FileName_, fbxFilename, sizeof(fbxFilename));

	FbxManager* fbxManager = FbxManager::Create();

	// FBX�ɑ΂�����o�͂��`����
	FbxIOSettings* fbxIOS = FbxIOSettings::Create(fbxManager, IOSROOT);	// ���ʂȗ��R���Ȃ�����IOSROOT���w��
	fbxManager->SetIOSettings(fbxIOS);

	// �C���|�[�^�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	bool result = fbxImporter->Initialize(fbxFilename, -1, fbxManager->GetIOSettings());	// -1�Ńt�@�C���t�H�[�}�b�g��������
	_ASSERT_EXPR_A(result, "FbxImporter::Initialize() : Failed!!\n");

	// Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();	// �V�[���𗬂����񂾂�Importer�͉������OK

	// ����Z-Up�̏ꍇ��Y-Up�ɕϊ�����
	FbxAxisSystem fbx_scene_axis_system = fbxScene->GetGlobalSettings().GetAxisSystem();
	if (fbx_scene_axis_system == FbxAxisSystem::eMayaZUp)
	{
		FbxAxisSystem::MayaYUp.ConvertScene(fbxScene);
	}

	// �A�j���[�V�����ǂݍ���
	char name[256];
	::_splitpath_s(FileName_, nullptr, 0, nullptr, 0, name, 256, nullptr, 0);

	char utf8[256];
	Encoding::StringToUTF8(name, utf8, sizeof(utf8));

	LoadAnimations(fbxScene, utf8, true);

	// �}�l�[�W�����
	fbxManager->Destroy();		// �֘A���邷�ׂẴI�u�W�F�N�g����������
}

void Resource::FbxModelResource::LoadNodes(FbxNode* pFbxNode_, int ParentNodeIndex_)
{
	const FbxNodeAttribute* fbxNodeAttribute = pFbxNode_->GetNodeAttribute();
	FbxNodeAttribute::EType fbxNodeAttributeType = FbxNodeAttribute::EType::eUnknown;

	if (fbxNodeAttribute != nullptr)
	{
		fbxNodeAttributeType = fbxNodeAttribute->GetAttributeType();
	}

	switch (fbxNodeAttributeType)
	{
	case FbxNodeAttribute::eUnknown:
	case FbxNodeAttribute::eNull:
	case FbxNodeAttribute::eMarker:
	case FbxNodeAttribute::eMesh:
	case FbxNodeAttribute::eSkeleton:
		LoadNode(pFbxNode_, ParentNodeIndex_);
		break;
	default:;
	}

	// �ċA�I�Ɏq�m�[�h����������
	ParentNodeIndex_ = static_cast<int>(mNodes.size() - 1);
	for (int i = 0; i < pFbxNode_->GetChildCount(); ++i)
	{
		LoadNodes(pFbxNode_->GetChild(i), ParentNodeIndex_);
	}
}

void Resource::FbxModelResource::LoadNode(FbxNode* pFbxNode_, int ParentNodeIndex_)
{
    const FbxAMatrix& fbxLocalTransform = pFbxNode_->EvaluateLocalTransform();

	Node node;
	node.mId = GetNodeId(pFbxNode_);
	node.mName = pFbxNode_->GetName();
	node.mPath = GetNodePath(pFbxNode_);
	node.mParentIndex = ParentNodeIndex_;
	node.mScale = FbxDouble4ToFloat3(fbxLocalTransform.GetS());
	node.mRotate = FbxDouble4ToFloat4(fbxLocalTransform.GetQ());
	node.mTranslate = FbxDouble4ToFloat3(fbxLocalTransform.GetT());

	// ���W���ϊ�
	ConvertTranslationFromRHtoLH(node.mTranslate);
	ConvertRotationFromRHtoLH(node.mRotate);

	mNodes.emplace_back(node);
}

void Resource::FbxModelResource::LoadMeshes(FbxNode* pFbxNode_)
{
	FbxNodeAttribute* fbxNodeAttribute = pFbxNode_->GetNodeAttribute();
	FbxNodeAttribute::EType fbxNodeAttributeType = FbxNodeAttribute::EType::eUnknown;

	if (fbxNodeAttribute != nullptr)
	{
		fbxNodeAttributeType = fbxNodeAttribute->GetAttributeType();
	}

	switch (fbxNodeAttributeType)
	{
	case FbxNodeAttribute::eMesh:
		LoadMesh(pFbxNode_, static_cast<FbxMesh*>(fbxNodeAttribute));
		break;
	default:;
	}

	// �ċA�I�Ɏq�m�[�h����������
	for (int i = 0; i < pFbxNode_->GetChildCount(); ++i)
	{
		LoadMeshes(pFbxNode_->GetChild(i));
	}
}

void Resource::FbxModelResource::LoadMesh(FbxNode* pFbxNode_, FbxMesh* pFbxMesh_)
{
	int fbxControlPointsCount = pFbxMesh_->GetControlPointsCount();
	//int fbxPolygonVertexCount = fbxMesh->GetPolygonVertexCount();
	//const int* fbxPolygonVertices = fbxMesh->GetPolygonVertices();

	int fbxMaterialCount = pFbxNode_->GetMaterialCount();
	int fbxPolygonCount = pFbxMesh_->GetPolygonCount();

	mMeshes.emplace_back(Mesh());
	Mesh& mesh = mMeshes.back();
	NodeId nodeId = GetNodeId(pFbxNode_);
	mesh.mNodeIndex = FindNodeIndex(nodeId);
	mesh.mSubsets.resize(fbxMaterialCount > 0 ? fbxMaterialCount : 1);
	mesh.mBoundsMin.x = mesh.mBoundsMin.y = mesh.mBoundsMin.z = FLT_MAX;;
	mesh.mBoundsMax.x = mesh.mBoundsMax.y = mesh.mBoundsMax.z = -FLT_MAX;
	// �T�u�Z�b�g�̃}�e���A���ݒ�
	for (int fbxMaterialIndex = 0; fbxMaterialIndex < fbxMaterialCount; ++fbxMaterialIndex)
	{
		const FbxSurfaceMaterial* fbxSurfaceMaterial = pFbxNode_->GetMaterial(fbxMaterialIndex);

		Subset& subset = mesh.mSubsets.at(fbxMaterialIndex);
		subset.mMaterialIndex = FindMaterialIndex(pFbxNode_->GetScene(), fbxSurfaceMaterial);
	}

	// �T�u�Z�b�g�̒��_�C���f�b�N�X�͈͐ݒ�
	if (fbxMaterialCount > 0)
	{
		for (int fbxPolygonIndex = 0; fbxPolygonIndex < fbxPolygonCount; ++fbxPolygonIndex)
		{
			int fbxMaterialIndex = pFbxMesh_->GetElementMaterial()->GetIndexArray().GetAt(fbxPolygonIndex);
			mesh.mSubsets.at(fbxMaterialIndex).mIndexCount += 3;
		}

		int offset = 0;
		for (Subset& subset : mesh.mSubsets)
		{
			subset.mStartIndex = offset;
			offset += subset.mIndexCount;

			subset.mIndexCount = 0;
		}
	}

	// ���_�e���̓f�[�^
	struct BoneInfluence
	{
		int useCount = 0;
		int indices[4] = { 0, 0, 0, 0 };
		float weights[4] = { 1.0f, 0.0f, 0.0f, 0.0f };

		void Add(int index, float weight)
		{
			if (useCount < 4)
			{
				indices[useCount] = index;
				weights[useCount] = weight;
				useCount++;
			}
		}
	};
	// ���_�e���̓f�[�^�𒊏o����
	std::vector<BoneInfluence> boneInfluences;
	{
		boneInfluences.resize(fbxControlPointsCount);

		// �X�L�j���O�ɕK�v�ȏ����擾����
		int fbxDeformerCount = pFbxMesh_->GetDeformerCount(FbxDeformer::eSkin);
		for (int fbxDeformerIndex = 0; fbxDeformerIndex < fbxDeformerCount; ++fbxDeformerIndex)
		{
			FbxSkin* fbxSkin = static_cast<FbxSkin*>(pFbxMesh_->GetDeformer(fbxDeformerIndex, FbxDeformer::eSkin));

			int fbxClusterCount = fbxSkin->GetClusterCount();
			for (int fbxClusterIndex = 0; fbxClusterIndex < fbxClusterCount; ++fbxClusterIndex)
			{
				FbxCluster* fbxCluster = fbxSkin->GetCluster(fbxClusterIndex);

				// ���_�e���̓f�[�^�𒊏o����
				{
					int fbxClusterControlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
					const int* fbxControlPointIndices = fbxCluster->GetControlPointIndices();
					const double* fbxControlPointWeights = fbxCluster->GetControlPointWeights();

					for (int i = 0; i < fbxClusterControlPointIndicesCount; ++i)
					{
						BoneInfluence& boneInfluence = boneInfluences.at(fbxControlPointIndices[i]);

						boneInfluence.Add(fbxClusterIndex, static_cast<float>(fbxControlPointWeights[i]));
					}
				}

				// ���b�V����Ԃ���{�[����Ԃւ̕ϊ��s��̌v�Z������
				{
					// ���b�V����Ԃ���O���[�o����Ԃւ̕ϊ��s��
					FbxAMatrix fbxMeshToGlobalSpaceTransform;
					fbxCluster->GetTransformMatrix(fbxMeshToGlobalSpaceTransform);

					// �{�[����Ԃ���O���[�o����Ԃւ̕ϊ��s��
					FbxAMatrix fbxBoneToGlobalSpaceTransform;
					fbxCluster->GetTransformLinkMatrix(fbxBoneToGlobalSpaceTransform);

					// ���b�V����Ԃ���{�[����Ԃւ̕ϊ��s��
					FbxAMatrix fbxMeshToBoneTransform = fbxBoneToGlobalSpaceTransform.Inverse() * fbxMeshToGlobalSpaceTransform;

					// ���W�n�ϊ�
					Math::Matrix offsetTransform = FbxAMatrixToFloat4x4(fbxMeshToBoneTransform);
					ConvertMatrixFromRHtoLH(offsetTransform);

					mesh.mOffsetTransforms.emplace_back(offsetTransform);

					// �e������{�[���C���f�b�N�X�𖼑O�Ō�������
					NodeId nodeId = GetNodeId(fbxCluster->GetLink());
					int nodeIndex = FindNodeIndex(nodeId);
					mesh.mNodeIndices.emplace_back(nodeIndex);
				}
			}
		}
	}

	// �W�I���g���ϊ��s��
	FbxAMatrix fbxGeometricTransform(
		pFbxNode_->GetGeometricTranslation(FbxNode::eSourcePivot),
		pFbxNode_->GetGeometricRotation(FbxNode::eSourcePivot),
		pFbxNode_->GetGeometricScaling(FbxNode::eSourcePivot)
	);
	DirectX::XMFLOAT4X4 geometricTransform = FbxAMatrixToFloat4x4(fbxGeometricTransform);
	DirectX::XMMATRIX GM = DirectX::XMLoadFloat4x4(&geometricTransform);

	// UV�Z�b�g��
	FbxStringList fbxUVSetNames;
	pFbxMesh_->GetUVSetNames(fbxUVSetNames);

	// ���_�f�[�^
	mesh.mVertices.resize(fbxPolygonCount * 3);
	mesh.mIndices.resize(fbxPolygonCount * 3);

	int vertexCount = 0;
	const FbxVector4* fbxControlPoints = pFbxMesh_->GetControlPoints();
	for (int fbxPolygonIndex = 0; fbxPolygonIndex < fbxPolygonCount; ++fbxPolygonIndex)
	{
		// �|���S���ɓK�p����Ă���}�e���A���C���f�b�N�X���擾����
		int fbxMaterialIndex = 0;
		if (fbxMaterialCount > 0)
		{
			fbxMaterialIndex = pFbxMesh_->GetElementMaterial()->GetIndexArray().GetAt(fbxPolygonIndex);
		}

		Subset& subset = mesh.mSubsets.at(fbxMaterialIndex);
		const int indexOffset = subset.mStartIndex + subset.mIndexCount;

		for (int fbxVertexIndex = 0; fbxVertexIndex < 3; ++fbxVertexIndex)
		{
			Vertex vertex;

			int fbxControlPointIndex = pFbxMesh_->GetPolygonVertex(fbxPolygonIndex, fbxVertexIndex);
			// Position
			{
				DirectX::XMFLOAT3 position = FbxDouble4ToFloat3(fbxControlPoints[fbxControlPointIndex]);
				DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&position);
				V = DirectX::XMVector3TransformCoord(V, GM);
				DirectX::XMStoreFloat3(&vertex.mPosition, V);
			}

			// Weight
			{
				BoneInfluence& boneInfluence = boneInfluences.at(fbxControlPointIndex);
				vertex.mBoneIndex.x = boneInfluence.indices[0];
				vertex.mBoneIndex.y = boneInfluence.indices[1];
				vertex.mBoneIndex.z = boneInfluence.indices[2];
				vertex.mBoneIndex.w = boneInfluence.indices[3];
				vertex.mBoneWeight.x = boneInfluence.weights[0];
				vertex.mBoneWeight.y = boneInfluence.weights[1];
				vertex.mBoneWeight.z = boneInfluence.weights[2];
				vertex.mBoneWeight.w = 1.0f - (vertex.mBoneWeight.x + vertex.mBoneWeight.y + vertex.mBoneWeight.z);
				//vertex.boneWeight.w = boneInfluence.weights[3];
			}

			// Normal
			if (pFbxMesh_->GetElementNormalCount() > 0)
			{
				FbxVector4 fbxNormal;
				pFbxMesh_->GetPolygonVertexNormal(fbxPolygonIndex, fbxVertexIndex, fbxNormal);

				DirectX::XMFLOAT3 normal = FbxDouble4ToFloat3(fbxNormal);
				DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&normal);
				V = DirectX::XMVector3TransformCoord(V, GM);
				V = DirectX::XMVector3Normalize(V);
				DirectX::XMStoreFloat3(&vertex.mNormal, V);

				vertex.mNormal = FbxDouble4ToFloat3(fbxNormal);
			}
			else
			{
				vertex.mNormal = Math::Vector3(0, 0, 0);
			}

			// Texcoord
			if (pFbxMesh_->GetElementUVCount() > 0)
			{
				bool fbxUnmappedUV;
				FbxVector2 fbxUV;
				pFbxMesh_->GetPolygonVertexUV(fbxPolygonIndex, fbxVertexIndex, fbxUVSetNames[0], fbxUV, fbxUnmappedUV);
				fbxUV[1] = 1.0 - fbxUV[1];
				vertex.mUv = FbxDouble2ToFloat2(fbxUV);
			}
			else
			{
				vertex.mUv = Math::Vector2(0, 0);
			}

			// ���W�n�ϊ�
			ConvertTranslationFromRHtoLH(vertex.mPosition);
			ConvertTranslationFromRHtoLH(vertex.mNormal);

			// ���_���C���f�b�N�X�f�[�^�ݒ�
			mesh.mIndices.at(indexOffset + fbxVertexIndex) = vertexCount;
			mesh.mVertices.at(vertexCount) = vertex;
			vertexCount++;

			// �o�E���f�B���O�{�b�N�X
			mesh.mBoundsMin.x = std::fminf(mesh.mBoundsMin.x, vertex.mPosition.x);
			mesh.mBoundsMin.y = std::fminf(mesh.mBoundsMin.y, vertex.mPosition.y);
			mesh.mBoundsMin.z = std::fminf(mesh.mBoundsMin.z, vertex.mPosition.z);
			mesh.mBoundsMax.x = std::fmaxf(mesh.mBoundsMax.x, vertex.mPosition.x);
			mesh.mBoundsMax.y = std::fmaxf(mesh.mBoundsMax.y, vertex.mPosition.y);
			mesh.mBoundsMax.z = std::fmaxf(mesh.mBoundsMax.z, vertex.mPosition.z);
		}

		subset.mIndexCount += 3;
	}

	// �^���W�F���g
	pFbxMesh_->GenerateTangentsData(0, true);
    if (auto* fbxElementTangent = pFbxMesh_->GetElementTangent(0); fbxElementTangent != nullptr)
	{
		int fbxPolygonVertexCount = pFbxMesh_->GetPolygonVertexCount();
		for (int i = 0; i < fbxPolygonVertexCount; i++)
		{
			FbxVector4 fbxTangent = fbxElementTangent->GetDirectArray().GetAt(i);
			mesh.mVertices[i].mTangent.x = static_cast<float>(fbxTangent[0]);
			mesh.mVertices[i].mTangent.y = static_cast<float>(fbxTangent[1]);
			mesh.mVertices[i].mTangent.z = static_cast<float>(fbxTangent[2]);

			// ���W�n�ϊ�
			ConvertTranslationFromRHtoLH(mesh.mVertices[i].mTangent);
		}
	}

	// ���_�J���[
	int fbxElementVertexColorLayerCount = pFbxMesh_->GetElementVertexColorCount();
	if (fbxElementVertexColorLayerCount > 0)
	{
		// ���_�J���[���C���[�擾
		FbxGeometryElementVertexColor* fbxElementVertexColor = pFbxMesh_->GetElementVertexColor(0);

		// �ۑ��`���̎擾
		FbxGeometryElement::EMappingMode fbxMappingMode = fbxElementVertexColor->GetMappingMode();
		FbxGeometryElement::EReferenceMode fbxReferenceMode = fbxElementVertexColor->GetReferenceMode();

		// �|���S�����_�ɑ΂���C���f�b�N�X�Q�ƌ`���̂ݑΉ�
		if (fbxMappingMode == FbxGeometryElement::eByPolygonVertex)
		{
			if (fbxReferenceMode == FbxGeometryElement::eIndexToDirect)
			{
				FbxLayerElementArrayTemplate<int>* indexArray = &fbxElementVertexColor->GetIndexArray();
				int indexCount = indexArray->GetCount();
				for (int i = 0; i < indexCount; i++)
				{
					FbxColor fbxColor = fbxElementVertexColor->GetDirectArray().GetAt(indexArray->GetAt(i));
					mesh.mVertices[i].mColor.x = static_cast<float>(fbxColor.mRed);
					mesh.mVertices[i].mColor.y = static_cast<float>(fbxColor.mGreen);
					mesh.mVertices[i].mColor.z = static_cast<float>(fbxColor.mBlue);
					mesh.mVertices[i].mColor.w = static_cast<float>(fbxColor.mAlpha);
				}
			}
		}
	}

#if 0	// �~���[�����O�̃��f���f�[�^�ł��܂������Ȃ��Ƃ�������
	// ���_�œK��
	int currentVertexCount = 0;
	vertexCount = static_cast<int>(mesh.vertices.size());
	for (int i = 0; i < vertexCount; ++i)
	{
		int sameIndex = -1;

		Vertex& a = mesh.vertices[i];
		//	���꒸�_����
		for (int j = 0; j < currentVertexCount; j++)
		{
			Vertex& b = mesh.vertices[j];

			if (a.position.x != b.position.x) continue;
			if (a.position.y != b.position.y) continue;
			if (a.position.z != b.position.z) continue;
			if (a.normal.x != b.normal.x) continue;
			if (a.normal.y != b.normal.y) continue;
			if (a.normal.z != b.normal.z) continue;
			if (a.tangent.x != b.tangent.x) continue;
			if (a.tangent.y != b.tangent.y) continue;
			if (a.tangent.z != b.tangent.z) continue;
			if (a.texcoord.x != b.texcoord.x) continue;
			if (a.texcoord.y != b.texcoord.y) continue;
			if (a.color.x != b.color.x) continue;
			if (a.color.y != b.color.y) continue;
			if (a.color.z != b.color.z) continue;
			if (a.color.w != b.color.w) continue;
			if (a.boneWeight.x != b.boneWeight.x) continue;
			if (a.boneWeight.y != b.boneWeight.y) continue;
			if (a.boneWeight.z != b.boneWeight.z) continue;
			if (a.boneWeight.w != b.boneWeight.w) continue;
			if (a.boneIndex.x != b.boneIndex.x) continue;
			if (a.boneIndex.y != b.boneIndex.y) continue;
			if (a.boneIndex.z != b.boneIndex.z) continue;
			if (a.boneIndex.w != b.boneIndex.w) continue;

			sameIndex = j;
			break;
		}

		int index = sameIndex;

		//	�V�K���_
		if (index == -1)
		{
			mesh.vertices[currentVertexCount] = a;
			index = currentVertexCount;
			currentVertexCount++;
		}
		//	�C���f�b�N�X�X�V
		for (int j = 0; j < vertexCount; ++j)
		{
			if (mesh.indices[j] == i)
			{
				mesh.indices[i] = index;
			}
		}
	}
	LOG("%s : vertex %d > %d\n", fbxNode->GetName(), mesh.vertices.size(), currentVertexCount);
	mesh.vertices.resize(currentVertexCount);
#endif

	// ���W�n�ϊ�
	ConvertIndexBufferFromRHtoLH(mesh.mIndices);
}

void Resource::FbxModelResource::LoadMaterials(const char* DirName_, FbxScene* pFbxScene)
{
	int fbxMaterialCount = pFbxScene->GetMaterialCount();

	if (fbxMaterialCount > 0)
	{
		for (int fbxMaterialIndex = 0; fbxMaterialIndex < fbxMaterialCount; ++fbxMaterialIndex)
		{
			FbxSurfaceMaterial* fbxSurfaceMaterial = pFbxScene->GetMaterial(fbxMaterialIndex);

			LoadMaterial(DirName_, fbxSurfaceMaterial);
		}
	}
	else
	{
		Material material;
		material.mName = "Dummy";
		material.mColor = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		mMaterials.emplace_back(material);
	}
}

void Resource::FbxModelResource::LoadMaterial(const char* DirName_,
    FbxSurfaceMaterial* fbxSurfaceMaterial)
{
	bool ret = false;

	Material material;

	material.mName = fbxSurfaceMaterial->GetName();

	// �f�B�t���[�Y�J���[
	FbxProperty fbxDiffuseProperty = fbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxProperty fbxDiffuseFactorProperty = fbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	if (fbxDiffuseProperty.IsValid() && fbxDiffuseFactorProperty.IsValid())
	{
		FbxDouble fbxFactor = fbxDiffuseFactorProperty.Get<FbxDouble>();
		FbxDouble3 fbxColor = fbxDiffuseProperty.Get<FbxDouble3>();

		material.mColor.x = static_cast<float>(fbxColor[0] * fbxFactor);
		material.mColor.y = static_cast<float>(fbxColor[1] * fbxFactor);
		material.mColor.z = static_cast<float>(fbxColor[2] * fbxFactor);
		material.mColor.w = 1.0f;
	}
#if 0
	FbxProperty fbx_transparency_factor_property = fbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (fbx_transparency_factor_property.IsValid())
	{
		FbxDouble fbxFactor = fbx_transparency_factor_property.Get<FbxDouble>();

		material.color.w = static_cast<float>(fbxFactor);
	}
#endif
	// �f�B�t���[�Y�e�N�X�`��
	if (fbxDiffuseProperty.IsValid())
	{
		int fbxTextureCount = fbxDiffuseProperty.GetSrcObjectCount<FbxFileTexture>();
		if (fbxTextureCount > 0)
		{
			FbxFileTexture* fbxTexture = fbxDiffuseProperty.GetSrcObject<FbxFileTexture>();

			const char* relativeFileName = fbxTexture->GetRelativeFileName();

			// �e�N�X�`���t�@�C�������݂��Ă��邩�m�F
			char filename[256];
			::_makepath_s(filename, sizeof(filename), nullptr, 
				DirName_, relativeFileName, nullptr);
			std::ifstream istream(filename, std::ios::binary);
			if (istream.is_open())
			{
				// ���������̂ł��̂܂ܐݒ肷��
				material.mTextureFileName = relativeFileName;
			}
			else
			{
				// ������Ȃ������ꍇ�̓��f���Ɠ��f�B���N�g���ɂ���Γǂݍ��߂�悤�ɂ���
				char fname[256], ext[32];
				::_splitpath_s(relativeFileName, nullptr, 0, nullptr, 0, fname, sizeof(fname), ext, sizeof(ext));
				::_makepath_s(filename, sizeof(filename), nullptr, nullptr, fname, ext);
				material.mTextureFileName = filename;
			}
		}
	}

	mMaterials.emplace_back(material);
}

void Resource::FbxModelResource::LoadAnimations(FbxScene* pFbxScene, const char* name, bool append)
{
	// ���ׂẴA�j���[�V���������擾
	FbxArray<FbxString*> fbxAnimStackNames;
	pFbxScene->FillAnimStackNameArray(fbxAnimStackNames);

    const int fbxAnimationCount = fbxAnimStackNames.Size();
	for (int fbxAnimationIndex = 0; fbxAnimationIndex < fbxAnimationCount; ++fbxAnimationIndex)
	{
		//if (0 != ::strncmp(fbxAnimStackNames.GetAt(fbxAnimationIndex)->Buffer(), u8"��{", strlen(u8"��{"))) continue;

		mAnimations.emplace_back(Animation());
		Animation& animation = mAnimations.back();

		// �A�j���[�V�����f�[�^�̃T���v�����O�ݒ�
		const FbxTime::EMode fbxTimeMode = pFbxScene->GetGlobalSettings().GetTimeMode();
		FbxTime fbxFrameTime;
		fbxFrameTime.SetTime(0, 0, 0, 1, 0, fbxTimeMode);

        const float samplingRate = static_cast<float>(fbxFrameTime.GetFrameRate(fbxTimeMode));
        const float samplingTime = 1.0f / samplingRate;

		FbxString* fbxAnimStackName = fbxAnimStackNames.GetAt(fbxAnimationIndex);
		FbxAnimStack* fbxAnimStack = pFbxScene->FindMember<FbxAnimStack>(fbxAnimStackName->Buffer());

		// �Đ�����A�j���[�V�������w�肷��B
		pFbxScene->SetCurrentAnimationStack(fbxAnimStack);

		// �A�j���[�V�����̍Đ��J�n���ԂƍĐ��I�����Ԃ��擾����
        const FbxTakeInfo* fbxTakeInfo = pFbxScene->GetTakeInfo(fbxAnimStackName->Buffer());
		FbxTime fbxStartTime = fbxTakeInfo->mLocalTimeSpan.GetStart();
		FbxTime fbxEndTime = fbxTakeInfo->mLocalTimeSpan.GetStop();

		// ���o����f�[�^��60�t���[����ŃT���v�����O����
		FbxTime fbxSamplingStep;
		fbxSamplingStep.SetTime(0, 0, 1, 0, 0, fbxTimeMode);
		fbxSamplingStep = static_cast<FbxLongLong>(fbxSamplingStep.Get() * samplingTime);

		int startFrame = static_cast<int>(fbxStartTime.Get() / fbxSamplingStep.Get());
		int endFrame = static_cast<int>(fbxEndTime.Get() / fbxSamplingStep.Get());
		int frameCount = static_cast<int>((fbxEndTime.Get() - fbxStartTime.Get()) / fbxSamplingStep.Get());

		// �A�j���[�V�����̑ΏۂƂȂ�m�[�h��񋓂���
		std::vector<FbxNode*> fbxNodes;
		if (append)
		{
			// �m�[�h�����r���đΏۃm�[�h��񋓂���
			// �����O���d�����Ă���Ǝ��s����ꍇ������
			FbxNode* fbxRootNode = pFbxScene->GetRootNode();
			for (Node& node : mNodes)
			{
				FbxNode* fbxNode = fbxRootNode->FindChild(node.mName.c_str(), true, true);
				fbxNodes.emplace_back(fbxNode);
			}
		}
		else
		{
			// �m�[�h�̊��S�p�X���r���đΏۃm�[�h��񋓂���i�d���j
			// ���K�����f���ƃA�j���[�V������FBX�̃m�[�h�c���[�\������v���Ă���K�v������
			for (const Node& node : mNodes)
			{

				FbxNode* fbxAnimationNode = nullptr;
				for (int fbxNodeIndex = 0; fbxNodeIndex < pFbxScene->GetNodeCount(); ++fbxNodeIndex)
				{
					FbxNode* fbxNode = pFbxScene->GetNode(fbxNodeIndex);
                    const NodeId nodeId = GetNodeId(fbxNode);
					if (node.mId == nodeId)
					{
						fbxAnimationNode = fbxNode;
						break;
					}
				}
				fbxNodes.emplace_back(fbxAnimationNode);
			}
		}

		// �A�j���[�V������
		char animationName[256];
		if (name != nullptr && fbxAnimationCount == 1)
		{
			::sprintf_s(animationName, "%s", name);
		}
		else
		{
			::sprintf_s(animationName, "%s", fbxAnimStackName->Buffer());
		}

		animation.mName = animationName;

		// �A�j���[�V�����f�[�^�𒊏o����
		animation.mSecondsLength = frameCount * samplingTime;
		animation.mKeyframes.resize(static_cast<std::vector<Keyframe, std::allocator<Keyframe>>::size_type>(frameCount) + 1);

		float seconds = 0.0f;
		Keyframe* keyframe = animation.mKeyframes.data();
		const size_t fbxNodeCount = fbxNodes.size();
		for (FbxTime fbxCurrentTime = fbxStartTime; fbxCurrentTime < fbxEndTime; fbxCurrentTime += fbxSamplingStep, ++keyframe)
		{
			// �L�[�t���[�����̎p���f�[�^�����o���B
			keyframe->mSeconds = seconds;
			keyframe->mNodeKeys.resize(fbxNodeCount);
			for (size_t fbxNodeIndex = 0; fbxNodeIndex < fbxNodeCount; ++fbxNodeIndex)
			{
				NodeKeyData& keyData = keyframe->mNodeKeys.at(fbxNodeIndex);
				FbxNode* fbxNode = fbxNodes.at(fbxNodeIndex);
				if (fbxNode == nullptr)
				{
					// �A�j���[�V�����Ώۂ̃m�[�h���Ȃ������̂Ń_�~�[�f�[�^��ݒ�
					Node& node = mNodes.at(fbxNodeIndex);
					keyData.mScale = node.mScale;
					keyData.mRotate = node.mRotate;
					keyData.mTranslate = node.mTranslate;
				}
				else if (fbxNodeIndex == mRootMotionNodeIndex)
				{
					// ���[�g���[�V�����͖�������
					keyData.mScale = Math::Vector3(1, 1, 1);
					keyData.mRotate = Math::Vector4(0, 0, 0, 1);
					keyData.mTranslate = Math::Vector3(0, 0, 0);
				}
				else
				{
					// �w�莞�Ԃ̃��[�J���s�񂩂�X�P�[���l�A��]�l�A�ړ��l�����o���B
					const FbxAMatrix& fbxLocalTransform = fbxNode->EvaluateLocalTransform(fbxCurrentTime);

					keyData.mScale = FbxDouble4ToFloat3(fbxLocalTransform.GetS());
					keyData.mRotate= FbxDouble4ToFloat4(fbxLocalTransform.GetQ());
					keyData.mTranslate = FbxDouble4ToFloat3(fbxLocalTransform.GetT());
				}
				// ���W�n�ϊ�
				ConvertTranslationFromRHtoLH(keyData.mTranslate);
				ConvertRotationFromRHtoLH(keyData.mRotate);
			}
			seconds += samplingTime;
		}
	}

	// ��n��
	for (int i = 0; i < fbxAnimationCount; i++)
	{
		delete fbxAnimStackNames[i];
	}
}

void Resource::FbxModelResource::ConvertAxisSystemFromRHtoLH()
{

#if 0
	for (Node& node : nodes)
	{
		node.translate.x = -node.translate.x;

		node.rotate.x = -node.rotate.x;
		node.rotate.w = -node.rotate.w;
	}

	for (Mesh& mesh : meshes)
	{
		for (Vertex& vertex : mesh.vertices)
		{
			vertex.position.x = -vertex.position.x;
		}
		for (DirectX::XMFLOAT4X4& offsetTransform : mesh.offsetTransforms)
		{
			DirectX::XMFLOAT4X4 tempTransform = offsetTransform;
			DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&offsetTransform);
			DirectX::XMVECTOR X = DirectX::XMVector3Length(M.r[0]);
			DirectX::XMVECTOR Y = DirectX::XMVector3Length(M.r[1]);
			DirectX::XMVECTOR Z = DirectX::XMVector3Length(M.r[2]);
			M.r[0] = DirectX::XMVector3Normalize(M.r[0]);
			M.r[1] = DirectX::XMVector3Normalize(M.r[1]);
			M.r[2] = DirectX::XMVector3Normalize(M.r[2]);
			DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationMatrix(M);
			DirectX::XMFLOAT4 q;
			DirectX::XMStoreFloat4(&q, Q);
			q.x = -q.x;
			q.w = -q.w;
			Q = DirectX::XMLoadFloat4(&q);
			M = DirectX::XMMatrixRotationQuaternion(Q);
			M.r[0] = DirectX::XMVectorMultiply(M.r[0], X);
			M.r[1] = DirectX::XMVectorMultiply(M.r[1], Y);
			M.r[2] = DirectX::XMVectorMultiply(M.r[2], Z);
			DirectX::XMStoreFloat4x4(&offsetTransform, M);

			//	offsetTransform._12 = tempTransform._21;
			//	offsetTransform._13 = tempTransform._31;
			//	offsetTransform._21 = tempTransform._12;
			//	offsetTransform._23 = tempTransform._32;
			//	offsetTransform._31 = tempTransform._13;
			//	offsetTransform._32 = tempTransform._23;
			offsetTransform._41 = -tempTransform._41;
			offsetTransform._42 = tempTransform._42;
			offsetTransform._43 = tempTransform._43;
		}
	}
#endif
}

std::string Resource::FbxModelResource::GetNodePath(FbxNode* pFbxNode_) const
{
	std::string parentNodeName;

	FbxNode* fbxParentNode = pFbxNode_->GetParent();
	if (fbxParentNode != nullptr)
	{
		parentNodeName = GetNodePath(fbxParentNode);
		return parentNodeName + "/" + pFbxNode_->GetName();
	}

	return pFbxNode_->GetName();
}

Resource::ModelResource::NodeId Resource::FbxModelResource::GetNodeId(FbxNode* pFbxNode_)
{
	return reinterpret_cast<NodeId>(pFbxNode_);
}

int Resource::FbxModelResource::FindMaterialIndex(FbxScene* pFbxScene, 
	const FbxSurfaceMaterial* pFbxSurfaceMaterial_)
{
	int fbxMaterialCount = pFbxScene->GetMaterialCount();

	for (int i = 0; i < fbxMaterialCount; ++i)
	{
		if (pFbxScene->GetMaterial(i) == pFbxSurfaceMaterial_)
		{
			return i;
		}
	}
	return -1;
}

void Resource::FbxModelResource::SetupIgnoreMotionNode(const char* IgnoreRootMotionNodeName_)
{
	// �������郋�[�g���[�V����������
	if (IgnoreRootMotionNodeName_ != nullptr)
	{
		mRootMotionNodeIndex = -1;
		for (size_t i = 0; i < mNodes.size(); ++i)
		{
			if (mNodes.at(i).mName == IgnoreRootMotionNodeName_)
			{
				mRootMotionNodeIndex = static_cast<int>(i);
				break;
			}
		}
	}
}

void Resource::FbxModelResource::ConvertTranslationFromRHtoLH(Math::Vector3& translate)
{
	translate.x = -translate.x;
}

void Resource::FbxModelResource::ConvertRotationFromRHtoLH(Math::Quaternion& rotate)
{
	rotate.x = -rotate.x;
	rotate.w = -rotate.w;
}

void Resource::FbxModelResource::ConvertMatrixFromRHtoLH(Math::Matrix& matrix)
{
#if 0
	DirectX::XMFLOAT4X4 temp = matrix;

	DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&matrix);

	// �X�P�[���l�����o��
	DirectX::XMVECTOR SX = DirectX::XMVector3Length(M.r[0]);
	DirectX::XMVECTOR SY = DirectX::XMVector3Length(M.r[1]);
	DirectX::XMVECTOR SZ = DirectX::XMVector3Length(M.r[2]);
	// �X�P�[���l�̂Ȃ��s��ɕϊ�
	M.r[0] = DirectX::XMVector3Normalize(M.r[0]);
	M.r[1] = DirectX::XMVector3Normalize(M.r[1]);
	M.r[2] = DirectX::XMVector3Normalize(M.r[2]);
	// ��]�s����N�H�[�^�j�I���֕ϊ�
	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationMatrix(M);
	DirectX::XMFLOAT4 q;
	DirectX::XMStoreFloat4(&q, Q);
	// �N�H�[�^�j�I����������W�n�֕ϊ�
	q.x = -q.x;
	q.w = -q.w;
	// �N�H�[�^�j�I������]�s��֕ϊ�
	Q = DirectX::XMLoadFloat4(&q);
	M = DirectX::XMMatrixRotationQuaternion(Q);
	// �X�P�[���l��߂�
	M.r[0] = DirectX::XMVectorMultiply(M.r[0], SX);
	M.r[1] = DirectX::XMVectorMultiply(M.r[1], SY);
	M.r[2] = DirectX::XMVectorMultiply(M.r[2], SZ);
	// ��]���X�P�[���s������o��
	DirectX::XMStoreFloat4x4(&matrix, M);
	// �ړ��l��������W�n�֕ϊ����ݒ�
	matrix._41 = -temp._41;
	matrix._42 = temp._42;
	matrix._43 = temp._43;
#else
matrix._12 = -matrix._12;
matrix._13 = -matrix._13;
matrix._21 = -matrix._21;
matrix._31 = -matrix._31;
matrix._41 = -matrix._41;
#endif
}

void Resource::FbxModelResource::ConvertIndexBufferFromRHtoLH(std::vector<UINT>& indices)
{
	size_t size = indices.size();
	UINT* p = indices.data();
	for (size_t i = 0; i < size; i += 3)
	{
		UINT temp = p[1];
		p[1] = p[2];
		p[2] = temp;

		p += 3;
	}
}
