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

        // 読み込み
		void Load(const char* FileName_, const char* IgnoreRootMotionNodeName_ = nullptr);

		// エクスポート
		void Export(const char* FileName_);
		// アニメーションの名前を書き出し
		void ExportAnimationText(const char* Filename_) const;

		// アニメーション追加読み込み
		void AddAnimation(const char* FileName_);

	private:
		// ノードデータを構築
		void LoadNodes(FbxNode* pFbxNode_, int ParentNodeIndex_);
		void LoadNode(FbxNode* pFbxNode_, int ParentNodeIndex_);

		// メッシュデータを読み込み
		void LoadMeshes(FbxNode* pFbxNode_);
		void LoadMesh(FbxNode* pFbxNode_, FbxMesh* pFbxMesh_);

		// マテリアルデータを読み込み
		void LoadMaterials(const char* DirName_, FbxScene* pFbxScene);
		void LoadMaterial(const char* DirName_, FbxSurfaceMaterial* fbxSurfaceMaterial);

		// アニメーションデータを読み込み
		void LoadAnimations(FbxScene* pFbxScene, const char* name, bool append);

		// 右手座標系から左手座標系へ変換
		void ConvertAxisSystemFromRHtoLH();

		// ノードパス取得
		std::string GetNodePath(FbxNode* pFbxNode_) const;

		// ノードID取得
		NodeId GetNodeId(FbxNode* pFbxNode_);

		// マテリアルインデックスを取得する
		int FindMaterialIndex(FbxScene* pFbxScene, const FbxSurfaceMaterial* pFbxSurfaceMaterial_);

		// 無視するモーションノード
		void SetupIgnoreMotionNode(const char* IgnoreRootMotionNodeName_);

		// 移動値を右手座標系から左手座標系へ変換する
		void ConvertTranslationFromRHtoLH(Math::Vector3 & translate);

		// 回転値を右手座標系から左手座標系へ変換する
		void ConvertRotationFromRHtoLH(Math::Quaternion& rotate);

		// 行列値を右手座標系から左手座標系へ変換する
		void ConvertMatrixFromRHtoLH(Math::Matrix& matrix);

		// インデックスバッファを右手座標系から左手座標系へ変換する
		void ConvertIndexBufferFromRHtoLH(std::vector<UINT>& indices);

	private:
		int						mRootMotionNodeIndex = -1;
	};
}
