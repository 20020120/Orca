#pragma once
// �`�掞�ɕ`��p�C�v���C���̏����o�C���h���ێ�����N���X
#include"GraphicsMacro.h"
#include"ShaderDesc.h"
#include"ShaderBuilder.h"

#include<map>
#include<tuple>
#include<d3d12.h>
#include<vector>
// ------------------------------------ �O���錾 -----------------------------------
struct ID3D12GraphicsCommandList;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Device;

namespace OrcaGraphics
{
    class RenderPipeline
    {
    public:
        // -------------------------------- ������ --------------------------------
        RenderPipeline(OrcaComPtr(ID3D12Device) pDevice_, const Shader::ShaderDesc& ShaderDesc_);

        RenderPipeline() = delete;
        ~RenderPipeline() = default;
        void StackGraphicsCmd(OrcaComPtr(ID3D12GraphicsCommandList) pCmdList_);
    private:
        OrcaComPtr(ID3D12RootSignature) mpRootSignature {};
        OrcaComPtr(ID3D12PipelineState) mpPipelineState {};
        Shader::ShaderDesc mShaderDesc{};

    private:

        // --------------------------- �f�B�X�N���v�^�����W���쐬 ---------------------------
        using DescriptorRanges = std::multimap<std::string, std::tuple<Shader::ShaderStage, D3D12_DESCRIPTOR_RANGE>>;
        using SamplerInfo = std::map<std::string, UINT>;   // �g���T���v���[�X�e�[�g�̏����擾����(���O/�o�C���h�X���b�g)

        void CreateRootParameter(const Shader::ShaderBuilder& ShaderBuilder_, std::vector<D3D12_ROOT_PARAMETER>& RootParameters_,
            DescriptorRanges& DescriptorRanges_, SamplerInfo& SamplersInfo_) const;
        static void CreateDescriptorRange(const Shader::ShaderBuilder& ShaderBuilder_, DescriptorRanges& DescriptorRanges_, SamplerInfo& SamplerInfo_);
    };
}
