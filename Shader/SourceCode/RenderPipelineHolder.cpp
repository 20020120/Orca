#include"RenderPipelineHolder.h"
#include"RenderPipline.h"
#include"MacroMemory.h"
#include<d3d12.h>
void OrcaGraphics::RenderPipeLineHolder::Set(const Shader::ShaderType& Type_, 
    ID3D12GraphicsCommandList* pCmdList_) const
{
    if (mCurrentShaderType == Type_)
        return;
    const auto& pPipeLine = mHolder.at(Type_);
    pPipeLine->StackGraphicsCmd(pCmdList_);
}

OrcaGraphics::RenderPipeLineHolder::RenderPipeLineHolder()
{
    // ---------------------------------- èâä˙âª ----------------------------------
    {
        Shader::ShaderDesc shaderDesc{};
        shaderDesc.mVsFileName = L"../Resource/Shader/ObjVs.cso";
        shaderDesc.mPsFileName = L"../Resource/Shader/ObjPs.cso";
        shaderDesc.mShaderType = Shader::ShaderType::Sample;
        shaderDesc.mBlendState = PipelineTypes::BlendState::Sample;
        shaderDesc.mRasterizerState = PipelineTypes::RasterizerState::Sample;
        shaderDesc.mDepthStencilState = PipelineTypes::DepthStencilState::Sample;
        mHolder.try_emplace(Shader::ShaderType::Sample, std::make_unique<RenderPipeline>(shaderDesc));
    }
    {
        Shader::ShaderDesc shaderDesc{};
        shaderDesc.mVsFileName = L"../Resource/Shader/lambertVs.cso";
        shaderDesc.mPsFileName = L"../Resource/Shader/lambertPs.cso";
        shaderDesc.mShaderType = Shader::ShaderType::Lambert;
        shaderDesc.mBlendState = PipelineTypes::BlendState::Sample;
        shaderDesc.mRasterizerState = PipelineTypes::RasterizerState::Sample;
        shaderDesc.mDepthStencilState = PipelineTypes::DepthStencilState::Sample;
        mHolder.try_emplace(Shader::ShaderType::Lambert, std::make_unique<RenderPipeline>(shaderDesc));
    }

}
