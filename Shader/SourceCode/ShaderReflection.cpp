#include "ShaderReflection.h"
#include<cassert>
#include <d3d12shader.h>
#include <d3dcompiler.h>
#include "GraphicsLogger.h"
#include"ReflectionHelpers.h"

OrcaGraphics::Shader::ShaderReflection::ReflectionData OrcaGraphics::Shader::ShaderReflection::Execute(
    const Microsoft::WRL::ComPtr<ID3DBlob> pBlobVs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobPs_,
    Microsoft::WRL::ComPtr<ID3DBlob> pBlobGs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobDs_,
    Microsoft::WRL::ComPtr<ID3DBlob> pBlobHs_)
{
    ReflectionData reflectionData;

    // -------------------------------- ���_�V�F�[�_�[ --------------------------------
    if(pBlobVs_)
    {
        // �V�F�[�_�[����K�v�ȏ����擾����
        Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector;
        auto hr = D3DReflect(pBlobVs_->GetBufferPointer(), pBlobVs_->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
        OrcaDebug::GraphicsLog("���_�V�F�[�_�[�̃��t���N�V���������擾", hr);

        // ���t���N�^�[��������擾
        D3D12_SHADER_DESC shaderDesc;
        pReflector->GetDesc(&shaderDesc);

        // �C���v�b�g���C�A�E�g���擾
        std::vector<D3D12_INPUT_ELEMENT_DESC> vbElement;
        for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i) {
            D3D12_SIGNATURE_PARAMETER_DESC sigDesc;
            pReflector->GetInputParameterDesc(i, &sigDesc);

            //�t�H�[�}�b�g���擾
            const auto format = ReflectionHelpers::GetDxgiFormat(sigDesc.ComponentType, sigDesc.Mask);

            D3D12_INPUT_ELEMENT_DESC elementDesc;
            const int semanticsLen = { static_cast<int>(std::strlen(sigDesc.SemanticName) + 1) };
            const auto pTmp = new char[semanticsLen]();
            std::memcpy(pTmp, sigDesc.SemanticName, semanticsLen);
            elementDesc.SemanticName = pTmp;
            elementDesc.SemanticIndex = 0;
            elementDesc.Format = format;
            elementDesc.InputSlot = 0;
            elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            elementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            elementDesc.InstanceDataStepRate = 0;
            reflectionData.mInputElementDescs.emplace_back(elementDesc);
        }
        // �C���v�b�g���C�A�E�g�̐���ݒ�
        reflectionData.mNumInputElements = shaderDesc.InputParameters;
        // �f�B�X�N���v�^�����W���擾
        for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
        {
            D3D12_SHADER_INPUT_BIND_DESC bindDesc;
            const auto hr = pReflector->GetResourceBindingDesc(i, &bindDesc);
            OrcaDebug::GraphicsLog("���\�[�X�̏����擾", hr);
            std::tuple tuple = { ShaderStage::Vertex,ReflectionHelpers::GetDescriptorRange(bindDesc) };
            reflectionData.mDescriptorRanges.emplace_back(tuple);
        }
    }
    // ------------------------------- �s�N�Z���V�F�[�_�[ -------------------------------
    if(pBlobPs_)
    {
        // �V�F�[�_�[����K�v�ȏ����擾����
        Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector;
        auto hr = D3DReflect(pBlobPs_->GetBufferPointer(), pBlobPs_->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
        OrcaDebug::GraphicsLog("�s�N�Z���V�F�[�_�[�̃��t���N�V���������擾", hr);

        // ���t���N�^�[��������擾
        D3D12_SHADER_DESC shaderDesc;
        pReflector->GetDesc(&shaderDesc);
        // �f�B�X�N���v�^�����W���擾
        for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
        {
            std::tuple tuple = { ShaderStage::Vertex,ReflectionHelpers::GetDescriptorRange(pReflector, i) };
            reflectionData.mDescriptorRanges.emplace_back(tuple);
        }
    }
    // -------------------------------- �h���C���V�F�[�_ -------------------------------
    if(pBlobDs_)
    {
        
    }
    // -------------------------------- �n���V�F�[�_�[ --------------------------------
    if(pBlobHs_)
    {
        
    }
    // ------------------------------- �W�I���g���V�F�[�_�[ ------------------------------
    if(pBlobGs_)
    {
        
    }

    return std::move(reflectionData);
}


// -------------------------- �V�F�[�_�[���t���N�V�����ŕԂ��p�̍\���̂̊֐� --------------------------
OrcaGraphics::Shader::ShaderReflection::ReflectionData::ReflectionData(ReflectionData&& RHS_) noexcept
{
    *this = RHS_;
    RHS_.mInputElementDescs.clear();
}

OrcaGraphics::Shader::ShaderReflection::ReflectionData::~ReflectionData()
{
    for (const auto& aaa : mInputElementDescs)
    {
        delete[] aaa.SemanticName;
    }
}
