#pragma once
#include<d3d12.h>
namespace OrcaGraphics
{
    namespace Shader
    {
        // 公開先クラスの前方宣言
        class Shader;
    }

    namespace PipelineObject
    {
        class BlendStates
        {
            // ---------------------------- 公開先のクラス ----------------------------
            friend class Shader::Shader;

            static D3D12_BLEND_DESC GetSampleBlendState();  // サンプル用のブレンドステート

            // レンダーターゲットのブレンド設定.
            inline static constexpr D3D12_RENDER_TARGET_BLEND_DESC mDescRtBs = {
                FALSE, FALSE,
                D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
                D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
                D3D12_LOGIC_OP_NOOP,
                D3D12_COLOR_WRITE_ENABLE_ALL
            };
        };
    }
}
