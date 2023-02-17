#include"BlendStates.h"
#include"OrcaException.h"
D3D12_BLEND_DESC OrcaGraphics::PipelineObject::BlendStates::GetBlendDesc(const Shader::ShaderDesc& ShaderDesc_)
{
    // ------------------------------ ブレンドステートを取得 ------------------------------
    switch (ShaderDesc_.m_BlendState)
    {
    case PipelineTypes::BlendState::Sample:
        return GetSampleBlendState();
    }

    // ここまで来たら未実装のステート
    Orca_Unimplemented
}

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
