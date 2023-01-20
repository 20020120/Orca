#pragma once
#include<d3d12.h>
#include<vector>
#include"ShaderBuilder.h"
namespace OrcaGraphics
{
    // ------------------------------ 公開先クラスの前方宣言 ------------------------------
    namespace InputLayout
    {
        class InputLayout
        {
            friend class RenderPipeline;
            // ------------------ D3D12_INPUT_ELEMENT_DESCを生成 ------------------
            static std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputElementDesc(const Shader::ShaderBuilder& ShaderBuilder_);
        };
    }
}
