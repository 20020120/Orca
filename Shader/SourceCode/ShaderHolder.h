#pragma once
#include"ShaderType.h"
#include"Shader.h"
#include<map>
#include<memory>

struct ID3D12GraphicsCommandList;

namespace System
{
    class RenderSystem;
}
namespace Graphics
{
    class ShaderHolder
    {
    public:
        void Set(const Shader::ShaderType& Type_, ID3D12GraphicsCommandList* pCmdList_) const;
    private:
        std::map<Shader::ShaderType, std::unique_ptr<Shader::Shader>> mHolder;
        Shader::ShaderType mCurrentShaderType{};

    public:
        ShaderHolder();
        ~ShaderHolder() = default;
        ShaderHolder& operator=(ShaderHolder&& RHS_) = delete;
        ShaderHolder operator=(const ShaderHolder& LHS_) = delete;
        ShaderHolder(ShaderHolder&& RHS_) = delete;
        ShaderHolder(const ShaderHolder& LHS_) = delete;
    };
}
