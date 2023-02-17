#include"BlendStates.h"
#include"OrcaException.h"
D3D12_BLEND_DESC OrcaGraphics::PipelineObject::BlendStates::GetBlendDesc(const Shader::ShaderDesc& ShaderDesc_)
{
    // ------------------------------ �u�����h�X�e�[�g���擾 ------------------------------
    switch (ShaderDesc_.m_BlendState)
    {
    case PipelineTypes::BlendState::Sample:
        return GetSampleBlendState();
    }

    // �����܂ŗ����疢�����̃X�e�[�g
    Orca_Unimplemented
}

D3D12_BLEND_DESC OrcaGraphics::PipelineObject::BlendStates::GetSampleBlendState()
{
    // �u�����h�X�e�[�g�̐ݒ�.
    D3D12_BLEND_DESC descBS;
    descBS.AlphaToCoverageEnable = FALSE;
    descBS.IndependentBlendEnable = FALSE;
    for (auto& i : descBS.RenderTarget)
    {
        i = mDescRtBs;
    }
    return descBS;
}
