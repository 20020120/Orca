#include"DepthStencilStates.h"

#include "OrcaException.h"
D3D12_DEPTH_STENCIL_DESC Graphics::PipelineObject::DepthStencilStates::GetDepthStencilState(
    const Shader::ShaderDesc& ShaderDesc_)
{
    switch (ShaderDesc_.m_DepthStencilState)
    {
    case PipelineTypes::DepthStencilState::Sample:
        return GetSampleDepthStencilState();
    }
    Orca_Unimplemented;
    return {};
}

D3D12_DEPTH_STENCIL_DESC Graphics::PipelineObject::DepthStencilStates::GetSampleDepthStencilState()
{
    D3D12_DEPTH_STENCIL_DESC descDSS{};
    descDSS.DepthEnable = TRUE;
    descDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    descDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    descDSS.StencilEnable = FALSE;
    return descDSS;
}
