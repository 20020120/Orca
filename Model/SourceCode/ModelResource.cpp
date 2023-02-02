#include "pch.h"
#include "ModelResource.h"


Model::ModelResource::ModelResource() = default;
Model::ModelResource::~ModelResource() = default;
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
