#pragma once
#include <d3d12.h>

namespace OrcaGraphics
{
    namespace Shader
    {
        // ���J��N���X�̑O���錾
        class Shader;
    }

    namespace PipelineObject
    {
        class RasterizerStates
        {
            // ---------------------------- ���J��̃N���X ----------------------------
            friend class Shader::Shader;

            static D3D12_RASTERIZER_DESC GetSampleRasterizerState();    // �T���v���p�̃��X�^���C�U�X�e�[�g��ݒ�


        };
    }
}
