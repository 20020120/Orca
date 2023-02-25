#include"Shader.h"
#include"Binary.h"
#include "GraphicsLogger.h"
#include "GraphicsMacro.h"
#include "OrcaException.h"

#include"ReflectionHelpers.h"

#include "SamplerStates.h"
#include"InputLayout.h"
#include"BlendStates.h"


#include<d3d12.h>
#include<filesystem>
#include <dxcapi.h>

#include "DepthStencilStates.h"
#include "Graphics.h"
#include "RasterizerStates.h"

OrcaGraphics::Shader::Shader::Data::~Data()
{
    if (m_pBin)
    {
        delete m_pBin;
        m_pBin = nullptr;
    }
    if (m_pRef)
    {
        delete m_pRef;
        m_pRef = nullptr;
    }
}
OrcaGraphics::Shader::Shader::Shader(IDxcUtils* pUtils_, const ShaderDesc& ShaderDesc_)
    :m_ShaderType(ShaderDesc_.m_ShaderType)
{
    {
        // ------------------------------ ���_�V�F�[�_�[��ǂݍ��� -----------------------------
        std::filesystem::path vsFileName = ShaderDesc_.m_VsFileName;
        OrcaBinary::Load(vsFileName.c_str(), &m_Vs.m_pBin, m_Vs.m_BinSize);
        OrcaBinary::Load(vsFileName.replace_extension("ref").c_str(), &m_Vs.m_pRef, m_Vs.m_RefSize);
    }
    {
        // ----------------------------- �s�N�Z���V�F�[�_�[��ǂݍ��� ----------------------------
        std::filesystem::path psFileName = ShaderDesc_.m_PsFileName;
        OrcaBinary::Load(psFileName.c_str(), &m_Ps.m_pBin, m_Ps.m_BinSize);
        OrcaBinary::Load(psFileName.replace_extension("ref").c_str(), &m_Ps.m_pRef, m_Ps.m_RefSize);
    }
    // ------------------------------ ���[�g�V�O�l�`�����쐬 ------------------------------
    CreateRootRootSignature(pUtils_, ShaderDesc_);
}

OrcaGraphics::Shader::Shader::~Shader()
{
   
}

void OrcaGraphics::Shader::Shader::Set(ID3D12GraphicsCommandList* pCmdList_)
{
    pCmdList_->SetGraphicsRootSignature(mpRootSignature.Get());
    pCmdList_->SetPipelineState(mpPipelineState.Get());
}

void OrcaGraphics::Shader::Shader::CreateRootRootSignature(IDxcUtils* pUtils_, const ShaderDesc& Desc_)
{
    std::vector<D3D12_ROOT_PARAMETER> rootParameters{};
    SamplerInfo samplersInfo{};
    DescriptorRanges descriptorRanges{};
    RootConstants rootConstants{};
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs{};

    // �f�B�X�N���v�^�����W���擾���邽�߂̃����_��
    auto getDescriptorRange = [&](ID3D12ShaderReflection* pReflector_, UINT BoundsResource_)
    {
        for (UINT i = 0; i < BoundsResource_; ++i)
        {
            D3D12_SHADER_INPUT_BIND_DESC bindDesc;
            const HRESULT hr = pReflector_->GetResourceBindingDesc(i, &bindDesc);
            OrcaDebug::GraphicsLog("���\�[�X�̏����擾", hr);
            auto descriptorRange = ReflectionHelpers::GetDescriptorRange(bindDesc);
            // �擾������񂪐ÓI�T���v���[�������ꍇ��p�̏����ɕ��򂷂�
            if (descriptorRange.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER) {
                samplersInfo.insert(std::make_pair(bindDesc.Name, descriptorRange.BaseShaderRegister));
                continue;
            }
            descriptorRanges.try_emplace(bindDesc.Name, descriptorRange);
            // �萔�o�b�t�@�̏ꍇ
            if (descriptorRange.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_CBV)
            {
                if(const auto cbData = pReflector_->GetConstantBufferByIndex(i))
                {
                    D3D12_SHADER_BUFFER_DESC desc;
                    cbData->GetDesc(&desc);
                    RootConstantInfo info{ desc,descriptorRange.RegisterSpace,descriptorRange.BaseShaderRegister };
                    rootConstants.try_emplace(desc.Name, info);
                }
            }
        }
    };

    // ----------------------------- ���t���N�V�����f�[�^���擾 -----------------------------
    {
        ID3D12ShaderReflection* pReflection;
        const DxcBuffer reflectionData{
            m_Vs.m_pRef,
            m_Vs.m_RefSize,
            DXC_CP_ACP, };
        if (FAILED(pUtils_->CreateReflection(&reflectionData, IID_PPV_ARGS(&pReflection)))) {
            Orca_Assert("���t���N�V�����f�[�^�̓ǂݎ��Ɏ��s���܂���");
        }
        D3D12_SHADER_DESC desc;
        pReflection->GetDesc(&desc);
        getDescriptorRange(pReflection, desc.BoundResources);
        // ----------------------------- �C���v�b�g���C�A�E�g���쐬 -----------------------------
        inputElementDescs = InputLayout::InputLayout::CreateInputElementDesc(&pReflection, desc);
    }
    {
        ID3D12ShaderReflection* pReflection;
        const DxcBuffer reflectionData{
            m_Ps.m_pRef,
            m_Ps.m_RefSize,
            DXC_CP_ACP, };
        if (FAILED(pUtils_->CreateReflection(&reflectionData, IID_PPV_ARGS(&pReflection)))) {
            Orca_Assert("���t���N�V�����f�[�^�̓ǂݎ��Ɏ��s���܂���");
        }
        D3D12_SHADER_DESC desc;
        pReflection->GetDesc(&desc);
        getDescriptorRange(pReflection, desc.BoundResources);
    }

    // ------------------------------ ���[�g�p�����[�^�[���쐬 -----------------------------
    for (auto it = descriptorRanges.begin(); it != descriptorRanges.end(); ++it)
    {
        auto& range = it->second;
        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;		// �p�����[�^���
        // TODO ��U�A�N�Z�X�w����I�[���ɂ��Ă��邪�ʐݒ�̂�����͍����邱��
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// �ǂ̃V�F�[�_�[���痘�p�\��
        param.DescriptorTable.NumDescriptorRanges = 1;					// �f�B�X�N���v�^�����W��
        param.DescriptorTable.pDescriptorRanges = &range;	            // �f�B�X�N���v�^�����W�̃A�h���X
        // ���[�g�萔�o�b�t�@�Ȃ�ǉ��̐ݒ���s��
        if(rootConstants.contains(it->first))
        {
            const auto& desc = rootConstants.at(it->first);
            param.Constants.Num32BitValues = desc.mShaderBufferDesc.Size;
            param.Constants.RegisterSpace = desc.mRegisterSpace;
            param.Constants.ShaderRegister = desc.mBaseShaderRegister;
            rootParameters.emplace_back(param);
        }
    }
    {
        // ------------------------------ �T���v���[�X�e�[�g���擾 -----------------------------
        auto samplerStates = PipelineObject::SamplerStates::GetStaticSamplerForReflection(samplersInfo);

        // ------------------------------ ���[�g�V�O�l�`���̐ݒ� ------------------------------
        D3D12_ROOT_SIGNATURE_DESC desc{};
        desc.NumParameters = static_cast<UINT>(rootParameters.size());
        desc.pParameters = rootParameters.data();
        desc.NumStaticSamplers = static_cast<UINT>(samplerStates.size());
        desc.pStaticSamplers = samplerStates.data();
        desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED| D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        // ---------------------------- ���[�g�V�O�l�`�����V���A���C�Y ----------------------------
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob{};
        Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{};
        auto hr = D3D12SerializeRootSignature(&desc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            pBlob.GetAddressOf(),
            pErrorBlob.GetAddressOf()
        );
        OrcaDebug::GraphicsLog("���[�g�V�O�l�`�����V���A���C�Y", hr);

        auto pDevice = Graphics::GetDevice();
        // ���[�g�V�O�l�`���𐶐�
        hr = pDevice->CreateRootSignature(
            0,
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            IID_PPV_ARGS(mpRootSignature.GetAddressOf())
        );
        mpRootSignature->SetName(L"Graphics");
        OrcaDebug::GraphicsLog("���[�g�V�O�l�`���𐶐�", hr);
    }
    // ----------------------------- �X�e�[�g�I�u�W�F�N�g���쐬 -----------------------------
    // �u�����h�X�e�[�g���擾
    auto blendDesc = PipelineObject::BlendStates::GetBlendDesc(Desc_);
    // ���X�^���C�U�X�e�[�g���擾
    auto rasterizerState = PipelineObject::RasterizerStates::GetRasterizerDesc(Desc_);
    // �f�v�X�X�e���V���X�e�[�g���擾
    auto depthStencilState = PipelineObject::DepthStencilStates::GetDepthStencilState(Desc_);

    // �O���t�B�b�N�X�p�C�v���C���I�u�W�F�N�g�𐶐�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descGps{};
    descGps.InputLayout = { inputElementDescs.data(),static_cast<UINT>(inputElementDescs.size()) };
    descGps.pRootSignature = mpRootSignature.Get();
    descGps.VS = { m_Vs.m_pBin,m_Vs.m_BinSize };
    descGps.PS = { m_Ps.m_pBin,m_Ps.m_BinSize };
    descGps.RasterizerState = rasterizerState;
    descGps.BlendState = blendDesc;
    descGps.DepthStencilState = depthStencilState;
    descGps.SampleMask = UINT_MAX;
    descGps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    descGps.NumRenderTargets = 1;
    descGps.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    descGps.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    descGps.SampleDesc = { 1,0 };

    // ----------------------------- �p�C�v���C���X�e�[�g��ݒ� -----------------------------
    auto pDevice = Graphics::GetDevice();
    auto hr = pDevice->CreateGraphicsPipelineState(
        &descGps, IID_PPV_ARGS(mpPipelineState.GetAddressOf()));
    OrcaDebug::GraphicsLog("�p�C�v���C���X�e�[�g���쐬", hr);

    for (const auto& aaa : inputElementDescs)
    {
        delete[] aaa.SemanticName;
    }
}
