#pragma once

#include"ShaderMinimun.h"
#include"GraphicsMacro.h"
#include"MacroMinmam.h"

#include <d3d12shader.h>
#include<string>
#include<d3dcommon.h>

// ------------------------------------ 前方宣言 -----------------------------------

// ------------------------------- シェーダーを作成するクラス -------------------------------
namespace OrcaGraphics
{
    namespace Shader
    {
        class ShaderBuilder
        {
        public:
            ShaderBuilder() = default;
            ShaderBuilder(const ShaderBuilder& RHS_) = default;
            ShaderBuilder& operator=(const ShaderBuilder&) = default;
            ~ShaderBuilder() = default;

            void Build(const wchar_t* FileName_, ShaderStage ShaderStage_);

            OrcaComPtr(ID3D12ShaderReflection) GetReflector()const;
            OrcaNodiscard D3D12_SHADER_DESC GetShaderDesc()const;
            OrcaNodiscard ShaderStage GetShaderStage()const;
            OrcaNodiscard OrcaComPtr(ID3DBlob) GetBlob()const;
        private:
            OrcaComPtr(ID3DBlob) mpBlob {};
            OrcaComPtr(ID3D12ShaderReflection) mpReflector {};
            D3D12_SHADER_DESC mShaderDesc{};

            ShaderStage mShaderStage{};
            std::wstring mFileName{};
        };
    }
}
