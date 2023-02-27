#pragma once
#include<d3d12.h>
#include<vector>
#include"ShaderBuilder.h"
namespace Graphics
{
    namespace Shader
    {
        class Shader;
    }

    // ------------------------------ ���J��N���X�̑O���錾 ------------------------------
    namespace InputLayout
    {
        class InputLayout
        {
            friend class RenderPipeline;
            friend Shader::Shader;
            // ------------------ D3D12_INPUT_ELEMENT_DESC�𐶐� ------------------
            static std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputElementDesc(const Shader::ShaderBuilder& ShaderBuilder_);
            static std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputElementDesc(ID3D12ShaderReflection** pReflector_, const D3D12_SHADER_DESC& Desc_);
        };
    }
}
