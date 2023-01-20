#pragma once
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcommon.h>
#include <dxgiformat.h>

namespace OrcaGraphics
{
    // ---------------------------------- 前方宣言 ---------------------------------
    namespace InputLayout
    {
        class InputLayout;
    }
    class RenderPipeline;

    namespace Shader
    {
        class ReflectionHelpers
        {
            // ----------------------------- 公開先クラス ----------------------------
            friend InputLayout::InputLayout;
            friend RenderPipeline;

            // --------------------- シェーダー情報を受け取るためのヘルパー関数 ---------------------
            // インプットレイアウトのフォーマットを取得
            static DXGI_FORMAT GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);

            // シェーダーリフレクションのリソースの種類を取得
            static D3D12_DESCRIPTOR_RANGE_TYPE GetDescriptorRangeType(D3D_SHADER_INPUT_TYPE Type_, D3D_SRV_DIMENSION Dimension_);
            static D3D12_DESCRIPTOR_RANGE GetDescriptorRange(D3D12_SHADER_INPUT_BIND_DESC BindDesc_);
        };
    }
}
