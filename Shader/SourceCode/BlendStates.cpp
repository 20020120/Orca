#include"BlendStates.h"

D3D12_BLEND_DESC OrcaGraphics::PipelineObject::BlendStates::GetSampleBlendState()
{
    // ブレンドステートの設定.
    D3D12_BLEND_DESC descBS;
    descBS.AlphaToCoverageEnable = FALSE;
    descBS.IndependentBlendEnable = FALSE;
    for (auto& i : descBS.RenderTarget)
    {
        i = mDescRtBs;
    }
    return descBS;
}
