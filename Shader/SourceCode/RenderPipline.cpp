#include"RenderPipline.h"
#include"ShaderBuilder.h"
#include"MacroMinmam.h"
#include"OrcaException.h"
#include "GraphicsLogger.h"

#include"InputLayout.h"
#include"BlendStates.h"
#include"SamplerStates.h"
#include"RasterizerStates.h"
#include"DepthStencilStates.h"

#include"ReflectionHelpers.h"

#include<filesystem>

OrcaGraphics::RenderPipeline::RenderPipeline(OrcaComPtr(ID3D12Device) pDevice_, const Shader::ShaderDesc& ShaderDesc_)
{
    // ------------------------------ ���_�V�F�[�_�[��ǂݍ��� -----------------------------
    OrcaExecuteDebug(
        if (!std::filesystem::exists(ShaderDesc_.mVsFileName))
            Orca_Assert("���_�V�F�[�_�[�t�@�C�����J���܂���");
    );
    Shader::ShaderBuilder vsBuilder{};
    vsBuilder.Build(ShaderDesc_.mVsFileName.c_str(), Shader::ShaderStage::VS);

    // ----------------------------- �s�N�Z���V�F�[�_�[��ǂݍ��� ----------------------------
    OrcaExecuteDebug(
        if (!std::filesystem::exists(ShaderDesc_.mPsFileName))
            Orca_Assert("�s�N�Z���V�F�[�_�[�t�@�C�����J���܂���");
    );
    Shader::ShaderBuilder psBuilder{};
    psBuilder.Build(ShaderDesc_.mPsFileName.c_str(), Shader::ShaderStage::PS);

    // ------------------------------ ���[�g�p�����[�^�[���擾 -----------------------------
    std::vector<D3D12_ROOT_PARAMETER> rootParameters{};
    SamplerInfo samplersInfo{};
    DescriptorRanges descriptorRanges{};

    // �f�B�X�N���v�^�����W���擾���邽�߂̃����_��
    auto getDescriptorRange = [&](OrcaComPtr(ID3D12ShaderReflection) pReflector, UINT BoundsResource_, Shader::ShaderStage ShaderStage_)
    {
        for (UINT i = 0; i < BoundsResource_; ++i)
        {
            D3D12_SHADER_INPUT_BIND_DESC bindDesc;
            HRESULT hr = pReflector->GetResourceBindingDesc(i, &bindDesc);
            OrcaDebug::GraphicsLog("���\�[�X�̏����擾", hr);
            auto descriptorRange = Shader::ReflectionHelpers::GetDescriptorRange(bindDesc);
            // �擾������񂪐ÓI�T���v���[�������ꍇ��p�̏����ɕ��򂷂�
            if (descriptorRange.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
            {
                samplersInfo.insert(std::make_pair(bindDesc.Name, descriptorRange.BaseShaderRegister));
                continue;
            }
            std::tuple tuple = { ShaderStage_,descriptorRange };
            descriptorRanges.insert(std::make_pair(bindDesc.Name, tuple));
        }
    };

    getDescriptorRange(vsBuilder.GetReflector(), vsBuilder.GetShaderDesc().BoundResources, Shader::ShaderStage::VS);
    getDescriptorRange(psBuilder.GetReflector(), psBuilder.GetShaderDesc().BoundResources, Shader::ShaderStage::PS);

    // ------------------------------ ���[�g�p�����[�^�[���쐬 -----------------------------
    for (auto it = descriptorRanges.begin(); it != descriptorRanges.end();)
    {
        // ----------------------------- ���O���ƂɃA�N�Z�X -----------------------------
        const UINT counts = static_cast<UINT>(descriptorRanges.count(it->first));
        {
            //auto nameIt = descriptorRanges.find(it->first);
            //UINT shaderStage{};
            //for (UINT i = 0; i < counts; ++i)
            //{
            //    const  auto& stage = std::get<0>(nameIt->second);
            //    shaderStage |= stage;
            //    ++nameIt;
            //}
        }

        auto& range = std::get<1>(it->second);
        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// �p�����[�^���
        //param.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(shaderStage); // �ǂ̃V�F�[�_�[���痘�p�\��
        // TODO ��U�A�N�Z�X�w����I�[���ɂ��Ă��邪�ʐݒ�̂�����͍����邱��
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// �ǂ̃V�F�[�_�[���痘�p�\��
        param.DescriptorTable.NumDescriptorRanges = 1;					// �f�B�X�N���v�^�����W��
        param.DescriptorTable.pDescriptorRanges = &range;	            // �f�B�X�N���v�^�����W�̃A�h���X
        rootParameters.emplace_back(param);
        // �d�����Ă��镪�����C�e���[�^��i�߂�
        for (UINT c = 0; c < counts; ++c)
        {
            ++it;
        }
    }

    // ------------------------ �����ȉ��̃V�F�[�_�[�͓ǂݍ��݂Ɏ��s���Ă������� -----------------------


    // ----------------------------- �C���v�b�g���C�A�E�g���쐬 -----------------------------
    auto inputElementDescs = InputLayout::InputLayout::CreateInputElementDesc(vsBuilder);
    UINT inputElementDescCount = static_cast<UINT>(inputElementDescs.size());

    // ------------------------------ �T���v���[�X�e�[�g���擾 -----------------------------
    auto samplerStates = PipelineObject::SamplerStates::GetStaticSamplerForReflection(samplersInfo);

    // ------------------------------ ���[�g�V�O�l�`���̐ݒ� ------------------------------
    D3D12_ROOT_SIGNATURE_DESC desc{};
    desc.NumParameters = static_cast<UINT>(rootParameters.size());
    desc.pParameters = rootParameters.data();
    desc.NumStaticSamplers = static_cast<UINT>(samplerStates.size());
    desc.pStaticSamplers = samplerStates.data();
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;



    Microsoft::WRL::ComPtr<ID3DBlob> pBlob{};
    Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{};
    // �V���A���C�Y
    auto hr = D3D12SerializeRootSignature(&desc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        pBlob.GetAddressOf(),
        pErrorBlob.GetAddressOf()
    );
    OrcaDebug::GraphicsLog("���[�g�V�O�l�`�����V���A���C�Y", hr);

    // ���[�g�V�O�l�`���𐶐�
    hr = pDevice_->CreateRootSignature(
        0,
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        IID_PPV_ARGS(mpRootSignature.GetAddressOf())
    );

    mpRootSignature->SetName(L"Graphics");
    OrcaDebug::GraphicsLog("���[�g�V�O�l�`���𐶐�", hr);

    // ----------------------------- �X�e�[�g�I�u�W�F�N�g���쐬 -----------------------------
  // �u�����h�X�e�[�g���擾
    auto blendDesc = PipelineObject::BlendStates::GetBlendDesc(ShaderDesc_);
    // ���X�^���C�U�X�e�[�g���擾
    auto rasterizerState = PipelineObject::RasterizerStates::GetRasterizerDesc(ShaderDesc_);
    // �f�v�X�X�e���V���X�e�[�g���擾
    auto depthStencilState = PipelineObject::DepthStencilStates::GetDepthStencilState(ShaderDesc_);



    // �O���t�B�b�N�X�p�C�v���C���I�u�W�F�N�g�𐶐�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descGps{};
    descGps.InputLayout = { inputElementDescs.data(),inputElementDescCount };
    descGps.pRootSignature = mpRootSignature.Get();
    descGps.VS = { vsBuilder.GetBlob()->GetBufferPointer(),vsBuilder.GetBlob()->GetBufferSize() };
    descGps.PS = { psBuilder.GetBlob()->GetBufferPointer(),psBuilder.GetBlob()->GetBufferSize() };
    descGps.RasterizerState = rasterizerState;
    descGps.BlendState = blendDesc;
    descGps.DepthStencilState = depthStencilState;
    descGps.SampleMask = UINT_MAX;
    descGps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    descGps.NumRenderTargets = 1;
    descGps.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    descGps.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    descGps.SampleDesc = { 1,0 };

    // �p�C�v���C���X�e�[�g��ݒ�
    hr = pDevice_->CreateGraphicsPipelineState(
        &descGps, IID_PPV_ARGS(mpPipelineState.GetAddressOf()));

    OrcaDebug::GraphicsLog("�p�C�v���C���X�e�[�g���쐬", hr);

    for (const auto& aaa : inputElementDescs)
    {
        delete[] aaa.SemanticName;
    }
}

void OrcaGraphics::RenderPipeline::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    pCmdList_->SetGraphicsRootSignature(mpRootSignature.Get());
    pCmdList_->SetPipelineState(mpPipelineState.Get());
}

void OrcaGraphics::RenderPipeline::CreateRootParameter(const Shader::ShaderBuilder& ShaderBuilder_, std::vector<D3D12_ROOT_PARAMETER>& RootParameters_, DescriptorRanges& DescriptorRanges_, SamplerInfo& SamplersInfo_) const
{
    // ----------------------------- �f�B�X�N���v�^�����W�𐶐� -----------------------------
    CreateDescriptorRange(ShaderBuilder_, DescriptorRanges_, SamplersInfo_);

    // ------------------------------ ���[�g�p�����[�^�[���쐬 -----------------------------
    for (auto it = DescriptorRanges_.begin(); it != DescriptorRanges_.end();)
    {
        // ----------------------------- ���O���ƂɃA�N�Z�X -----------------------------
        const UINT counts = static_cast<UINT>(DescriptorRanges_.count(it->first));
        {
            //auto nameIt = descriptorRanges.find(it->first);
            //UINT shaderStage{};
            //for (UINT i = 0; i < counts; ++i)
            //{
            //    const  auto& stage = std::get<0>(nameIt->second);
            //    shaderStage |= stage;
            //    ++nameIt;
            //}
        }

        auto& range = std::get<1>(it->second);
        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// �p�����[�^���
        //param.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(shaderStage); // �ǂ̃V�F�[�_�[���痘�p�\��
        // TODO ��U�A�N�Z�X�w����I�[���ɂ��Ă��邪�ʐݒ�̂�����͍����邱��
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// �ǂ̃V�F�[�_�[���痘�p�\��
        param.DescriptorTable.NumDescriptorRanges = 1;					// �f�B�X�N���v�^�����W��
        param.DescriptorTable.pDescriptorRanges = &range;	            // �f�B�X�N���v�^�����W�̃A�h���X
        RootParameters_.emplace_back(param);
        // �d�����Ă��镪�����C�e���[�^��i�߂�
        for (UINT c = 0; c < counts; ++c)
        {
            ++it;
        }
    }
}

void OrcaGraphics::RenderPipeline::CreateDescriptorRange(const Shader::ShaderBuilder& ShaderBuilder_,
    DescriptorRanges& DescriptorRanges_, SamplerInfo& SamplerInfo_)
{
    // ----------------------------- �f�B�X�N���v�^�����W���쐬 -----------------------------
       // ���t���N�^�[��������擾
    const D3D12_SHADER_DESC shaderDesc = ShaderBuilder_.GetShaderDesc();
    const auto pReflector = ShaderBuilder_.GetReflector();

    for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
    {
        D3D12_SHADER_INPUT_BIND_DESC bindDesc;
        HRESULT hr = pReflector->GetResourceBindingDesc(i, &bindDesc);
        OrcaDebug::GraphicsLog("���\�[�X�̏����擾", hr);
        auto descriptorRange = Shader::ReflectionHelpers::GetDescriptorRange(bindDesc);
        // �擾������񂪐ÓI�T���v���[�������ꍇ��p�̏����ɕ��򂷂�
        if (descriptorRange.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
        {
            SamplerInfo_.insert(std::make_pair(bindDesc.Name, descriptorRange.BaseShaderRegister));
            continue;
        }
        std::tuple tuple = { ShaderBuilder_.GetShaderStage(),descriptorRange };
        DescriptorRanges_.insert(std::make_pair(bindDesc.Name, tuple));
    }
}
