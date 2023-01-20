#pragma once
#include"ShaderDesc.h"
#include <d3d12.h>
namespace OrcaGraphics
{
    // ------------------------------ ���J��N���X�̑O���錾 ------------------------------
    namespace Shader
    {
        class Shader;
    }
    class RenderPipeline;

    namespace PipelineObject
    {
        class RasterizerStates
        {
            // ---------------------------- ���J��̃N���X ----------------------------
            friend class Shader::Shader;
            friend class RenderPipeline;

            // ���X�^���C�U�X�e�[�g���擾
            static D3D12_RASTERIZER_DESC GetRasterizerDesc(const Shader::ShaderDesc& ShaderDesc_);

            static D3D12_RASTERIZER_DESC GetSampleRasterizerState();    // �T���v���p�̃��X�^���C�U�X�e�[�g��ݒ�


        };
    }
}
