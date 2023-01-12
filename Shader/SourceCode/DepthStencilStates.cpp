#include"DepthStencilStates.h"

D3D12_DEPTH_STENCIL_DESC OrcaGraphics::PipelineObject::DepthStencilStates::GetSampleDepthStencilState()
{
    D3D12_DEPTH_STENCIL_DESC descDSS{};
    descDSS.DepthEnable = TRUE;
    descDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    descDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    descDSS.StencilEnable = FALSE;
    return descDSS;
}
