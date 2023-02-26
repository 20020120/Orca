#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Dx12VertexBuffer.h"
#include "Dx12IndexBuffer.h"


#include <string>
#include <vector>
#include <DirectXMath.h>

#include<cereal/types/vector.hpp>
#include<cereal/types/string.hpp>
struct ID3D12GraphicsCommandList;

// -------------------------------- モデルのリソースデータ --------------------------------
namespace Model
{
    struct ModelResource
    {
		ModelResource();
		virtual ~ModelResource();
		using NodeId = UINT64;

		struct Node
		{
			NodeId				mId;
			std::string			mName;
			std::string			mPath;
			int					mParentIndex;
			Math::Vector3	mScale;
			Math::Quaternion	mRotate;
			Math::Vector3	mTranslate;

			template<class Archive>
			void serialize(Archive& archive, int version)
			{
				archive(mId, mName, mPath, mParentIndex, mScale, mRotate, mTranslate);
			}
		};

		struct Material
		{
			std::string			mName;
			std::string			mTextureFileName;
			Math::Vector4	mColor = { 0.8f, 0.8f, 0.8f, 1.0f };
			
			template<class Archive>
			void serialize(Archive& archive, int version)
			{
				archive(mName, mTextureFileName, mColor);
			}
		};

		struct Subset
		{
			UINT		mStartIndex = 0;
			UINT		mIndexCount = 0;
			int			mMaterialIndex = 0;

			Material* material = nullptr;

			template<class Archive>
			void serialize(Archive& archive, int version)
			{
				archive(mStartIndex, mIndexCount, mMaterialIndex);
			}
		};

		struct Vertex
		{
			Math::Vector3	mPosition = { 0, 0, 0 };
			Math::Vector3	mNormal = { 0, 0, 0 };
			Math::Vector3	mTangent = { 0, 0, 0 };
			Math::Vector2	mUv = { 0, 0 };
			Math::Vector4	mColor = { 1, 1, 1, 1 };
			Math::Vector4	mBoneWeight = { 1, 0, 0, 0 };
			DirectX::XMUINT4	mBoneIndex = { 0, 0, 0, 0 };

			template<class Archive>
			void serialize(Archive& archive, int version)
			{
				archive(mPosition, mNormal, mTangent, mUv, mColor, mBoneWeight);
			}
		};

		struct Mesh
		{
			std::vector<Vertex>						mVertices;
			std::vector<UINT>						mIndices;
			std::vector<Subset>						mSubsets;

			int										mNodeIndex;
			std::vector<int>						mNodeIndices;
			std::vector<Math::Matrix>		mOffsetTransforms;

			Math::Vector3						mBoundsMin;
			Math::Vector3						mBoundsMax;

			OrcaGraphics::Resource::VertexBuffer mVertexBuffer{};
			OrcaGraphics::Resource::IndexBuffer mIndexBuffer{};

			void StackGraphicsCmd(ID3D12GraphicsCommandList* pCmdList_)const;

			template<class Archive>
			void serialize(Archive& archive, int version)
			{
				archive(mVertices, mIndices,
					mSubsets, mNodeIndex,
					mNodeIndices, mOffsetTransforms,
					mBoundsMin, mBoundsMax);
			}
		};

		struct NodeKeyData
		{
			Math::Vector3	mScale;
			Math::Quaternion 	mRotate;
			Math::Vector3	mTranslate;

			template<class Archive>
			void serialize(Archive& archive, int version)
			{
				archive(mScale, mRotate, mTranslate);
			}
		};

		struct Keyframe
		{
			float						mSeconds;
			std::vector<NodeKeyData>	mNodeKeys;

			template<class Archive>
			void serialize(Archive& archive, int version)
			{
				archive(mSeconds, mNodeKeys);
			}
		};
		struct Animation
		{
			std::string					mName;
			float						mSecondsLength;
			std::vector<Keyframe>		mKeyframes;

			template<class Archive>
			void serialize(Archive& archive, int version)
			{
				archive(mName, mSecondsLength, mKeyframes);
			}
		};

		// 読み込み
		//void Load(ID3D12Device* device, const char* filename);

		// 各種データ取得
		[[nodiscard]]const std::vector<Mesh>& GetMeshes() const { return mMeshes; }
		[[nodiscard]]const std::vector<Node>& GetNodes() const { return mNodes; }
		[[nodiscard]]const std::vector<Animation>& GetAnimations() const { return mAnimations; }
		[[nodiscard]]const std::vector<Material>& GetMaterials() const { return mMaterials; }

	protected:
		// モデルセットアップ
		//void BuildModel(ID3D12Device* device, const char* dirname);

		// シリアライズ
		void Serialize(const char* filename);

		// デシリアライズ
		void Deserialize(const char* filename);

		// ノードインデックスを取得する
		int FindNodeIndex(NodeId nodeId) const;

    public:
		std::vector<Node>		mNodes;
		std::vector<Material>	mMaterials;
		std::vector<Mesh>		mMeshes;
		std::vector<Animation>	mAnimations;
    };
}
