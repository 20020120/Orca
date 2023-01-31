//#pragma once
//#include"Vector2.h"
//#include"Vector3.h"
//#include "Vector4.h"
//#include"Quaternion.h"
//
//#include <string>
//#include <vector>
//#include <wrl.h>
//#include <d3d12.h>
//#include <DirectXMath.h>
//
//// -------------------------------- モデルのリソースデータ --------------------------------
//namespace Model
//{
//    struct ModelResource
//    {
//		using NodeId = UINT64;
//
//		struct Node
//		{
//			NodeId				mId;
//			std::string			mName;
//			std::string			mPath;
//			int					mParentIndex;
//			Math::Vector3	mScale;
//			Math::Quaternion	mRotate;
//			Math::Vector3	mTranslate;
//
//			//template<class Archive>
//			//void serialize(Archive& archive, int version);
//		};
//
//		struct Material
//		{
//			std::string			mName;
//			std::string			mTextureFileName;
//			Math::Vector4	mColor = { 0.8f, 0.8f, 0.8f, 1.0f };
//
//			//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
//			//void LoadTexture(ID3D11Device* device, const char* filename);
//
//			//template<class Archive>
//			//void serialize(Archive& archive, int version);
//		};
//
//		struct Subset
//		{
//			UINT		mStartIndex = 0;
//			UINT		mIndexCount = 0;
//			int			mMaterialIndex = 0;
//
//			Material* material = nullptr;
//
//			//template<class Archive>
//			//void serialize(Archive& archive, int version);
//		};
//
//		struct Vertex
//		{
//			Math::Vector3	mPosition = { 0, 0, 0 };
//			Math::Vector3	mNormal = { 0, 0, 0 };
//			Math::Vector3	mTangent = { 0, 0, 0 };
//			Math::Vector2	mUv = { 0, 0 };
//			Math::Vector4	mColor = { 1, 1, 1, 1 };
//			Math::Vector4	mBoneWeight = { 1, 0, 0, 0 };
//			DirectX::XMUINT4	mBoneIndex = { 0, 0, 0, 0 };
//
//			//template<class Archive>
//			//void serialize(Archive& archive, int version);
//		};
//
//		struct Mesh
//		{
//			std::vector<Vertex>						vertices;
//			std::vector<UINT>						indices;
//			std::vector<Subset>						subsets;
//
//			int										nodeIndex;
//			std::vector<int>						nodeIndices;
//			std::vector<DirectX::XMFLOAT4X4>		offsetTransforms;
//
//			DirectX::XMFLOAT3						boundsMin;
//			DirectX::XMFLOAT3						boundsMax;
//
//			Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
//			Microsoft::WRL::ComPtr<ID3D11Buffer>	indexBuffer;
//
//			template<class Archive>
//			void serialize(Archive& archive, int version);
//		};
//
//		struct NodeKeyData
//		{
//			DirectX::XMFLOAT3	scale;
//			DirectX::XMFLOAT4	rotate;
//			DirectX::XMFLOAT3	translate;
//
//			template<class Archive>
//			void serialize(Archive& archive, int version);
//		};
//
//		struct Keyframe
//		{
//			float						seconds;
//			std::vector<NodeKeyData>	nodeKeys;
//
//			template<class Archive>
//			void serialize(Archive& archive, int version);
//		};
//		struct Animation
//		{
//			std::string					name;
//			float						secondsLength;
//			std::vector<Keyframe>		keyframes;
//
//			template<class Archive>
//			void serialize(Archive& archive, int version);
//		};
//
//		// 読み込み
//		void Load(ID3D11Device* device, const char* filename);
//
//		// 各種データ取得
//		const std::vector<Mesh>& GetMeshes() const { return meshes; }
//		const std::vector<Node>& GetNodes() const { return nodes; }
//		const std::vector<Animation>& GetAnimations() const { return animations; }
//		const std::vector<Material>& GetMaterials() const { return materials; }
//
//	protected:
//		// モデルセットアップ
//		void BuildModel(ID3D11Device* device, const char* dirname);
//
//		// シリアライズ
//		void Serialize(const char* filename);
//
//		// デシリアライズ
//		void Deserialize(const char* filename);
//
//		// ノードインデックスを取得する
//		int FindNodeIndex(NodeId nodeId) const;
//
//	protected:
//		std::vector<Node>		nodes;
//		std::vector<Material>	materials;
//		std::vector<Mesh>		meshes;
//		std::vector<Animation>	animations;
//
//    };
//}
