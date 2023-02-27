#include"ShaderHolder.h"
#include"MacroMemory.h"

#include"Shader.h"
#include"ShaderCompiler.h"
#include<d3d12.h>
#include <dxcapi.h>

void Graphics::ShaderHolder::Set(const Shader::ShaderType& Type_,
                                             ID3D12GraphicsCommandList* pCmdList_) const
{
    if (mCurrentShaderType == Type_)
        return;
    const auto& pPipeLine = mHolder.at(Type_);
    pPipeLine->Set(pCmdList_);
}

Graphics::ShaderHolder::ShaderHolder()
{
    // コンパイラとユーティリティを生成
    OrcaComPtr(IDxcUtils) pUtils;
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));

    // デバッグ時のみシェーダーをコンパイル
#ifdef _DEBUG
    ShaderCompiler::AllCompile("../HLSL/SourceCode/", "../Resource/Shader/", "../Resource/Settings/Setting.txt");
#endif
    // ---------------------------------- 初期化 ----------------------------------
    {
        Shader::ShaderDesc shaderDesc{};
        shaderDesc.m_VsFileName = L"../Resource/Shader/ObjVs.bin";
        shaderDesc.m_PsFileName = L"../Resource/Shader/ObjPs.bin";
        shaderDesc.m_ShaderType = Shader::ShaderType::Sample;
        shaderDesc.m_BlendState = PipelineTypes::BlendState::Sample;
        shaderDesc.m_RasterizerState = PipelineTypes::RasterizerState::Sample;
        shaderDesc.m_DepthStencilState = PipelineTypes::DepthStencilState::Sample;
        mHolder.try_emplace(Shader::ShaderType::Sample, std::make_unique<Shader::Shader>(pUtils.Get(), shaderDesc));
    }
    
    {
        Shader::ShaderDesc shaderDesc{};
        shaderDesc.m_VsFileName = L"../Resource/Shader/lambertVs.bin";
        shaderDesc.m_PsFileName = L"../Resource/Shader/lambertPs.bin";
        shaderDesc.m_ShaderType = Shader::ShaderType::Lambert;
        shaderDesc.m_BlendState = PipelineTypes::BlendState::Sample;
        shaderDesc.m_RasterizerState = PipelineTypes::RasterizerState::Sample;
        shaderDesc.m_DepthStencilState = PipelineTypes::DepthStencilState::Sample;
        mHolder.try_emplace(Shader::ShaderType::Lambert, std::make_unique<Shader::Shader>(pUtils.Get(), shaderDesc));
    }

}
