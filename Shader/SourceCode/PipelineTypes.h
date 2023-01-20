#pragma once

// パイプラインステートの種類
namespace OrcaGraphics
{
    namespace PipelineTypes
    {
        // ------------------------------ ブレンドステート -----------------------------
        enum class BlendState
        {
            Sample,
        };
        // ----------------------------- ラスタライザステート ----------------------------
        enum class RasterizerState
        {
            Sample,
        };
        // ---------------------------- デプスステンシルステート ---------------------------
        enum class DepthStencilState
        {
            Sample,
        };
    }
}
