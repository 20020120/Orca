#pragma once
#include<string>
// ---------------------------- シェーダーのリソースを生成するための情報 ---------------------------
namespace Graphics
{
    namespace Resource
    {
        enum class ResourceType
        {
            Unknown,
            ConstantBuffer,
            Sampler,
            Texture,
            UAV,
        };

        struct Dx12ResourceInfo
        {
            ResourceType mType{};   // リソースの種類
            std::string mName{};    // リソースの名前（HLSLでの変数名）
            size_t mBufferSize{};   // バッファの大きさ
            uint32_t mRootIndex{};  // バインド時のインデックス
        };
    }
}
