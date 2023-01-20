#pragma once
#include<d3d12.h>
#include<vector>
#include"ShaderBuilder.h"
namespace OrcaGraphics
{
    // ------------------------------ ���J��N���X�̑O���錾 ------------------------------
    namespace InputLayout
    {
        class InputLayout
        {
            friend class RenderPipeline;
            // ------------------ D3D12_INPUT_ELEMENT_DESC�𐶐� ------------------
            static std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputElementDesc(const Shader::ShaderBuilder& ShaderBuilder_);
        };
    }
}
