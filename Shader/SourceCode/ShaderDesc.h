#pragma once
// �V�F�[�_�[���g���X�e�[�g����ݒ肷��t�@�C���i���͒������ł��������ǌ�X�O���t�@�C���Ƃ��ɂ������j

#include"ShaderType.h"
#include"PipelineTypes.h"

#include<string>
namespace OrcaGraphics
{
    namespace Shader
    {
        struct ShaderDesc
        {
            ShaderType mShaderType{ ShaderType::Unknown };
            std::wstring mVsFileName{};
            std::wstring mPsFileName{};
            std::wstring mGsFileName{};
            std::wstring mDsFileName{};
            std::wstring mHsFileName{};
            std::wstring mCsFileName{};
            
            PipelineTypes::BlendState mBlendState{};
            PipelineTypes::DepthStencilState mDepthStencilState{};
            PipelineTypes::RasterizerState mRasterizerState{};
        };
    }
}
