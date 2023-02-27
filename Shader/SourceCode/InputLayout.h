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

    // ------------------------------ 公開先クラスの前方宣言 ------------------------------
    namespace InputLayout
    {
        class InputLayout
        {
            friend class RenderPipeline;
            friend Shader::Shader;
            // ------------------ D3D12_INPUT_ELEMENT_DESCを生成 ------------------
            static std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputElementDesc(const Shader::ShaderBuilder& ShaderBuilder_);
            static std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputElementDesc(ID3D12ShaderReflection** pReflector_, const D3D12_SHADER_DESC& Desc_);
        };
    }
}
