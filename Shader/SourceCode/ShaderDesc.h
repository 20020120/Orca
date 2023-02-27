#pragma once
// �V�F�[�_�[���g���X�e�[�g����ݒ肷��t�@�C���i���͒������ł��������ǌ�X�O���t�@�C���Ƃ��ɂ������j

#include"ShaderType.h"
#include"PipelineTypes.h"

#include<string>
namespace Graphics
{
    namespace Shader
    {
        struct ShaderDesc
        {
            ShaderType m_ShaderType{ ShaderType::Unknown };
            std::wstring m_VsFileName{};
            std::wstring m_PsFileName{};
            std::wstring m_GsFileName{};
            std::wstring m_DsFileName{};
            std::wstring m_HsFileName{};
            std::wstring m_CsFileName{};
            
            PipelineTypes::BlendState m_BlendState{};
            PipelineTypes::DepthStencilState m_DepthStencilState{};
            PipelineTypes::RasterizerState m_RasterizerState{};
        };
    }
}
