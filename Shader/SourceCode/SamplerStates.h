#pragma once
#include <d3d12.h>
#include<vector>
#include<map>
#include<string>
namespace OrcaGraphics
{
    namespace Shader
    {
        // ���J��N���X�̑O���錾
        class Shader;
    }


    namespace PipelineObject
    {
        class SamplerStates
        {
            // ---------------------------- ���J��̃N���X ----------------------------
            friend class Shader::Shader;
            friend class RenderPipeline;

            // �V�F�[�_�[���t���N�V�����Ŏ擾������񂩂�ÓI�T���v���[�X�e�[�g�𐶐����ĕԂ��֐�
            static std::vector<D3D12_STATIC_SAMPLER_DESC> GetStaticSamplerForReflection(const std::map<std::string, UINT>
                & reflectionData_);

            // �X�^�e�B�b�N�T���v���[���擾
            static D3D12_STATIC_SAMPLER_DESC GetStaticSampler_Sample(); // �T���v���p�̐ÓI�T���v���[
            static D3D12_STATIC_SAMPLER_DESC GetStaticSampler_LinerClamp(UINT Slot_);  // ���j�A�T���v���[
        };
    }
}
