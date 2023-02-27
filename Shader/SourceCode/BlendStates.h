#pragma once
#include"ShaderDesc.h"
#include<d3d12.h>
namespace Graphics
{
    // ------------------------------ 公開先クラスの前方宣言 ------------------------------
    namespace Shader
    {
        class Shader;
    }
    class RenderPipeline;

    namespace PipelineObject
    {
        class BlendStates
        {
            // ---------------------------- 公開先のクラス ----------------------------
            friend class Shader::Shader;
            friend class RenderPipeline;

            // -------------------------- ブレンドステートを取得 --------------------------
            static D3D12_BLEND_DESC GetBlendDesc(const Shader::ShaderDesc& ShaderDesc_);


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
