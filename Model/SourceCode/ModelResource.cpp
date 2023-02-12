#include "pch.h"
#include "ModelResource.h"


Model::ModelResource::ModelResource() = default;
Model::ModelResource::~ModelResource() = default;

void Model::ModelResource::Mesh::StackGraphicsCmd(ID3D12GraphicsCommandList* pCmdList_) const
{
	// 頂点バッファ設定
	pCmdList_->IASetVertexBuffers(0, 1, &mVertexBuffer.mVbView);
	pCmdList_->IASetIndexBuffer(&mIndexBuffer.mIbView);
	pCmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Model::ModelResource::FindNodeIndex(NodeId nodeId) const
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
