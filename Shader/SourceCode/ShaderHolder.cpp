#include"ShaderHolder.h"
#include"RenderPipline.h"
#include"MacroMemory.h"

#include"Shader.h"
#include<d3d12.h>
#include <dxcapi.h>

void OrcaGraphics::ShaderHolder::Set(const Shader::ShaderType& Type_,
                                             ID3D12GraphicsCommandList* pCmdList_) const
{
    if (mCurrentShaderType == Type_)
        return;
    const auto& pPipeLine = mHolder.at(Type_);
    pPipeLine->Set(pCmdList_);
}

OrcaGraphics::ShaderHolder::ShaderHolder()
{
    // コンパイラとユーティリティを生成
    OrcaComPtr(IDxcUtils) pUtils;
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));


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
    
    //{
    //    Shader::ShaderDesc shaderDesc{};
    //    shaderDesc.m_VsFileName = L"../Resource/Shader/lambertVs.cso";
    //    shaderDesc.m_PsFileName = L"../Resource/Shader/lambertPs.cso";
    //    shaderDesc.m_ShaderType = Shader::ShaderType::Lambert;
    //    shaderDesc.m_BlendState = PipelineTypes::BlendState::Sample;
    //    shaderDesc.m_RasterizerState = PipelineTypes::RasterizerState::Sample;
    //    shaderDesc.m_DepthStencilState = PipelineTypes::DepthStencilState::Sample;
    //    mHolder.try_emplace(Shader::ShaderType::Lambert, std::make_unique<RenderPipeline>(shaderDesc));
    //}

}
