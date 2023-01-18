// ReSharper disable CppClangTidyClangDiagnosticPessimizingMove
#include "Shader.h"
#include "GraphicsLogger.h"
#include"ShaderReflection.h"

#include "SamplerStates.h"
#include"BlendStates.h"
#include"DepthStencilStates.h"
#include"RasterizerStates.h"

#include<d3d12.h>
#include <d3dcompiler.h>
#include <ranges>


void OrcaGraphics::Shader::Shader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const char* VsPath_, const char* PsPath_)
{
    HRESULT hr{};
    // ���_�V�F�[�_�[�ǂݍ���
    Microsoft::WRL::ComPtr<ID3DBlob> pVsBlob{};
    hr = D3DReadFileToBlob(L"../Resource/Shader/ObjVs.cso", pVsBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("���_�V�F�[�_�[��ǂݍ���", hr);

    // �s�N�Z���V�F�[�_�[�ǂݍ���
    Microsoft::WRL::ComPtr<ID3DBlob> pPsBlob{};
    hr = D3DReadFileToBlob(L"../Resource/Shader/ObjPs.cso", pPsBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("�s�N�Z���V�F�[�_�[��ǂݍ���", hr);

    // �V�F�[�_�[���烊�t���N�V�������g���ď����擾
    auto reflectionData = ShaderReflection::Execute(pVsBlob,pPsBlob);

    // ------------------------------ ���[�g�p�����[�^�[��ݒ� -----------------------------
    std::vector<D3D12_ROOT_PARAMETER> rootParameters;
    UINT rootIndex{}; // ���[�g�p�����[�^�[�̒ʂ��ԍ�
    for (auto it = reflectionData.mDescriptorRanges.begin(); it != reflectionData.mDescriptorRanges.end();)
    {
        // ----------------------------- ���O���ƂɃA�N�Z�X -----------------------------
        auto nameIt = reflectionData.mDescriptorRanges.find(it->first);
        UINT shaderStage{};
        UINT counts = static_cast<UINT>(reflectionData.mDescriptorRanges.count(it->first));
        for (UINT i = 0; i < counts; ++i)
        {
            const  auto& stage = std::get<0>(nameIt->second);
            shaderStage |= stage;
            ++nameIt;
        }

        auto& range = std::get<1>(it->second);
        D3D12_ROOT_PARAMETER param;
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// �p�����[�^���
        //param.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(shaderStage); // �ǂ̃V�F�[�_�[���痘�p�\��
        // TODO ��U�A�N�Z�X�w����I�[���ɂ��Ă��邪�ʐݒ�̂�����͍����邱��
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			// �ǂ̃V�F�[�_�[���痘�p�\��
        param.DescriptorTable.NumDescriptorRanges = 1;							// �f�B�X�N���v�^�����W��
        param.DescriptorTable.pDescriptorRanges = &range;	                    // �f�B�X�N���v�^�����W�̃A�h���X
        rootParameters.emplace_back(param);
        // ���O�ƃC���f�b�N�X�����\�[�X�o�C���h�̂��߂ɕێ�����
        mRootParamIndexes.try_emplace(it->first, rootIndex++);
        // �d�����Ă��镪�����C�e���[�^��i�߂�
        for (UINT c = 0; c < counts; ++c)
        {
            ++it;
        }
    }
    // �X�^�e�B�b�N�T���v���[��ݒ肷��
    auto samplers = PipelineObject::SamplerStates::GetStaticSamplerForReflection(reflectionData.SamplerStatesInfo);

    // ���[�g�V�O�l�`���̐ݒ�
    D3D12_ROOT_SIGNATURE_DESC desc{};
    desc.NumParameters = static_cast<UINT>(rootParameters.size());
    desc.pParameters = rootParameters.data();
    desc.NumStaticSamplers = static_cast<UINT>(samplers.size());
    desc.pStaticSamplers = samplers.data();
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob{};
    Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{};
    // �V���A���C�Y
    hr = D3D12SerializeRootSignature(&desc,
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


    // ���X�^���C�U�[�X�e�[�g�̐ݒ�
    auto rasterizerState = PipelineObject::RasterizerStates::GetSampleRasterizerState();
    // �f�v�X�X�e���V���X�e�[�g��ݒ�
    auto depthStencilState = PipelineObject::DepthStencilStates::GetSampleDepthStencilState();
    // �u�����h�X�e�[�g���擾
    auto blendState = PipelineObject::BlendStates::GetSampleBlendState();


    // �O���t�B�b�N�X�p�C�v���C���I�u�W�F�N�g�𐶐�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descGps{};
    descGps.InputLayout = { reflectionData.mInputElementDescs.data(), reflectionData.mNumInputElements };
    descGps.pRootSignature = mpRootSignature.Get();
    descGps.VS = { pVsBlob->GetBufferPointer(),pVsBlob->GetBufferSize() };
    descGps.PS = { pPsBlob->GetBufferPointer(),pPsBlob->GetBufferSize() };
    descGps.RasterizerState = rasterizerState;
    descGps.BlendState = blendState;
    descGps.DepthStencilState = depthStencilState;
    descGps.SampleMask = UINT_MAX;
    descGps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    descGps.NumRenderTargets = 1;
    descGps.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    descGps.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    descGps.SampleDesc = { 1,0 };

    // �p�C�v���C���X�e�[�g��ݒ�
    hr = pDevice_->CreateGraphicsPipelineState(
        &descGps, IID_PPV_ARGS(mpPso.GetAddressOf()));

    OrcaDebug::GraphicsLog("�p�C�v���C���X�e�[�g���쐬", hr);
}

void OrcaGraphics::Shader::Shader::StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_)
{
    pCmdList_->SetGraphicsRootSignature(mpRootSignature.Get());
    pCmdList_->SetPipelineState(mpPso.Get());
}
