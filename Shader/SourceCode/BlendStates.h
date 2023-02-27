#pragma once
#include"ShaderDesc.h"
#include<d3d12.h>
namespace Graphics
{
    // ------------------------------ ���J��N���X�̑O���錾 ------------------------------
    namespace Shader
    {
        class Shader;
    }
    class RenderPipeline;

    namespace PipelineObject
    {
        class BlendStates
        {
            // ---------------------------- ���J��̃N���X ----------------------------
            friend class Shader::Shader;
            friend class RenderPipeline;

            // -------------------------- �u�����h�X�e�[�g���擾 --------------------------
            static D3D12_BLEND_DESC GetBlendDesc(const Shader::ShaderDesc& ShaderDesc_);


            static D3D12_BLEND_DESC GetSampleBlendState();  // �T���v���p�̃u�����h�X�e�[�g

            // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�.
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
