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
        class SamplerStates
        {
            // ---------------------------- 公開先のクラス ----------------------------
            friend class Shader::Shader;

            // スタティックサンプラーを取得
            static D3D12_STATIC_SAMPLER_DESC GetStaticSampler();


        };
    }
}
