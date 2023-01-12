#pragma once
#include<d3d12.h>
namespace OrcaGraphics
{
    namespace Shader
    {
        // ���J��N���X�̑O���錾
        class Shader;
    }

    namespace PipelineObject
    {
        class BlendStates
        {
            // ---------------------------- ���J��̃N���X ----------------------------
            friend class Shader::Shader;

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
