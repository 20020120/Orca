#pragma once
#include"ShaderDesc.h"
#include <d3d12.h>
namespace OrcaGraphics
{
    // ------------------------------ 公開先クラスの前方宣言 ------------------------------
    namespace Shader
    {
        class Shader;
    }
    class RenderPipeline;

    namespace PipelineObject
    {
        class RasterizerStates
        {
            // ---------------------------- 公開先のクラス ----------------------------
            friend class Shader::Shader;
            friend class RenderPipeline;

            // ラスタライザステートを取得
            static D3D12_RASTERIZER_DESC GetRasterizerDesc(const Shader::ShaderDesc& ShaderDesc_);

            static D3D12_RASTERIZER_DESC GetSampleRasterizerState();    // サンプル用のラスタライザステートを設定


        };
    }
}
