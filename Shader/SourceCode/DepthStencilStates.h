#pragma once
#include"ShaderDesc.h"
#include<d3d12.h>
namespace OrcaGraphics
{
    namespace Shader
    {
        // ���J��N���X�̑O���錾
        class Shader;
    }
    class RenderPipeline;

    namespace PipelineObject
    {
        class DepthStencilStates
        {
            // ---------------------------- ���J��̃N���X ----------------------------
            friend class Shader::Shader;
            friend class RenderPipeline;

            static D3D12_DEPTH_STENCIL_DESC GetDepthStencilState(const Shader::ShaderDesc& ShaderDesc_);

            static D3D12_DEPTH_STENCIL_DESC GetSampleDepthStencilState();    // �T���v���p�̃f�v�X�X�e���V���X�e�[�g���擾
           
        };

       
      
    }
}
