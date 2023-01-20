#pragma once
// シェーダーが使うステート情報を設定するファイル（今は直書きでもいいけど後々外部ファイルとかにしたい）

#include"ShaderType.h"
#include"PipelineTypes.h"

#include<string>
namespace OrcaGraphics
{
    namespace Shader
    {
        struct ShaderDesc
        {
            ShaderType mShaderType{ ShaderType::Unknown };
            std::wstring mVsFileName{};
            std::wstring mPsFileName{};
            std::wstring mGsFileName{};
            std::wstring mDsFileName{};
            std::wstring mHsFileName{};
            std::wstring mCsFileName{};
            
            PipelineTypes::BlendState mBlendState{};
            PipelineTypes::DepthStencilState mDepthStencilState{};
            PipelineTypes::RasterizerState mRasterizerState{};
        };
    }
}
