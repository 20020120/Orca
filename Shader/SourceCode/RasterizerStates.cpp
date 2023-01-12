#include"RasterizerStates.h"

D3D12_RASTERIZER_DESC OrcaGraphics::PipelineObject::RasterizerStates::GetSampleRasterizerState()
{
    D3D12_RASTERIZER_DESC descRS{};
    descRS.FillMode = D3D12_FILL_MODE_SOLID;
    descRS.CullMode = D3D12_CULL_MODE_NONE;
    descRS.FrontCounterClockwise = FALSE;
    descRS.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    descRS.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    descRS.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    descRS.DepthClipEnable = FALSE;
    descRS.MultisampleEnable = FALSE;
    descRS.AntialiasedLineEnable = FALSE;
    descRS.ForcedSampleCount = 0;
    descRS.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    return descRS;
}