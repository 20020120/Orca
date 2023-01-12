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
        class SamplerStates
        {
            // ---------------------------- ���J��̃N���X ----------------------------
            friend class Shader::Shader;

            // �X�^�e�B�b�N�T���v���[���擾
            static D3D12_STATIC_SAMPLER_DESC GetStaticSampler();


        };
    }
}
