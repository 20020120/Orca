#pragma once
#include"ShaderType.h"
#include"RenderPipline.h"
#include<map>
#include<memory>

struct ID3D12GraphicsCommandList;

namespace System
{
    class RenderSystem;
}
namespace OrcaGraphics
{
    class RenderPipeLineHolder
    {
    public:
        void Set(const Shader::ShaderType& Type_, ID3D12GraphicsCommandList* pCmdList_) const;
    private:
        std::map<Shader::ShaderType, std::unique_ptr<RenderPipeline>> mHolder;
        Shader::ShaderType mCurrentShaderType{};

    public:
        RenderPipeLineHolder();
        ~RenderPipeLineHolder() = default;
        RenderPipeLineHolder& operator=(RenderPipeLineHolder&& RHS_) = delete;
        RenderPipeLineHolder operator=(const RenderPipeLineHolder& LHS_) = delete;
        RenderPipeLineHolder(RenderPipeLineHolder&& RHS_) = delete;
        RenderPipeLineHolder(const RenderPipeLineHolder& LHS_) = delete;
    };
}
