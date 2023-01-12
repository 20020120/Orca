#pragma once
#include <d3d12.h>

namespace OrcaGraphics
{
    namespace Shader
    {
        // 公開先クラスの前方宣言
        class Shader;
    }

    namespace PipelineObject
    {
        class RasterizerStates
        {
            // ---------------------------- 公開先のクラス ----------------------------
            friend class Shader::Shader;

            static D3D12_RASTERIZER_DESC GetSampleRasterizerState();    // サンプル用のラスタライザステートを設定


        };
    }
}
