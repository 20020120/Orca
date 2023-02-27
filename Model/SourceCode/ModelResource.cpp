#include "pch.h"
#include "ModelResource.h"

#include<fstream>
#include<cereal/cereal.hpp>
#include<cereal/archives/binary.hpp>
#include<cereal/types/vector.hpp>

Resource::ModelResource::ModelResource() = default;
Resource::ModelResource::~ModelResource() = default;

void Resource::ModelResource::Mesh::StackGraphicsCmd(ID3D12GraphicsCommandList* pCmdList_) const
{
	// 頂点バッファ設定
	pCmdList_->IASetVertexBuffers(0, 1, &mVertexBuffer.mVbView);
	pCmdList_->IASetIndexBuffer(&mIndexBuffer.mIbView);
	pCmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Resource::ModelResource::FindNodeIndex(NodeId nodeId) const
{
    const int nodeCount = static_cast<int>(mNodes.size());
	for (int i = 0; i < nodeCount; ++i)
	{
		if (mNodes[i].mId == nodeId)
		{
			return i;
		}
	}
	return -1;
}

// シリアライズ
void Resource::ModelResource::Serialize(const char* filename)
{
    if (std::ofstream ofs(filename, std::ios::binary); ofs.is_open())
	{
		cereal::BinaryOutputArchive archive(ofs);

		archive(
			CEREAL_NVP(mNodes),
			CEREAL_NVP(mMaterials),
			CEREAL_NVP(mMeshes),
			CEREAL_NVP(mAnimations)
		);
	}
}

void Resource::ModelResource::Deserialize(const char* filename)
{
	if (std::ifstream ifs(filename, std::ios::binary); ifs.is_open())
	{
		cereal::BinaryInputArchive archive(ifs);

		archive(
			CEREAL_NVP(mNodes),
			CEREAL_NVP(mMaterials),
			CEREAL_NVP(mMeshes),
			CEREAL_NVP(mAnimations)
		);
	}
}
