//#include "ShaderReflection.h"
//#include<cassert>
//#include <d3d12shader.h>
//#include <d3dcompiler.h>
//#include "GraphicsLogger.h"
//#include"ReflectionHelpers.h"
//#include"GraphicsMacro.h"
//
//OrcaGraphics::Shader::ShaderReflection::ReflectionData OrcaGraphics::Shader::ShaderReflection::Execute(
//    const Microsoft::WRL::ComPtr<ID3DBlob> pBlobVs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobPs_,
//    Microsoft::WRL::ComPtr<ID3DBlob> pBlobGs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobDs_,
//    Microsoft::WRL::ComPtr<ID3DBlob> pBlobHs_)
//{
//    ReflectionData reflectionData;
//
//    // �f�B�X�N���v�^�����W���擾���邽�߂̃����_��
//    auto getDescriptorRange = [&](OrcaComPtr(ID3D12ShaderReflection) pReflector ,UINT BoundsResource_, D3D12_SHADER_VISIBILITY ShaderStage_)
//    {
//        for (UINT i = 0; i < BoundsResource_; ++i)
//        {
//            D3D12_SHADER_INPUT_BIND_DESC bindDesc;
//            HRESULT hr = pReflector->GetResourceBindingDesc(i, &bindDesc);
//            OrcaDebug::GraphicsLog("���\�[�X�̏����擾", hr);
//            auto descriptorRange = ReflectionHelpers::GetDescriptorRange(bindDesc);
//            // �擾������񂪐ÓI�T���v���[�������ꍇ��p�̏����ɕ��򂷂�
//            if (descriptorRange.RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
//            {
//                reflectionData.SamplerStatesInfo.insert(std::make_pair(bindDesc.Name, descriptorRange.BaseShaderRegister));
//                continue;
//            }
//            std::tuple tuple = { ShaderStage_,descriptorRange };
//            reflectionData.mDescriptorRanges.insert(std::make_pair(bindDesc.Name, tuple));
//        }
//    };
//
//    // -------------------------------- ���_�V�F�[�_�[ --------------------------------
//    if(pBlobVs_)
//    {
//        // �V�F�[�_�[����K�v�ȏ����擾����
//        Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector;
//        auto hr = D3DReflect(pBlobVs_->GetBufferPointer(), pBlobVs_->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
//        OrcaDebug::GraphicsLog("���_�V�F�[�_�[�̃��t���N�V���������擾", hr);
//
//        // ���t���N�^�[��������擾
//        D3D12_SHADER_DESC shaderDesc;
//        pReflector->GetDesc(&shaderDesc);
//
//        // �C���v�b�g���C�A�E�g���擾
//        std::vector<D3D12_INPUT_ELEMENT_DESC> vbElement;
//        for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i) {
//            D3D12_SIGNATURE_PARAMETER_DESC sigDesc;
//            pReflector->GetInputParameterDesc(i, &sigDesc);
//
//            //�t�H�[�}�b�g���擾
//            const auto format = ReflectionHelpers::GetDxgiFormat(sigDesc.ComponentType, sigDesc.Mask);
//            D3D12_INPUT_ELEMENT_DESC elementDesc;
//            const int semanticsLen = { static_cast<int>(std::strlen(sigDesc.SemanticName) + 1) };
//            const auto pTmp = new char[semanticsLen]();
//            std::memcpy(pTmp, sigDesc.SemanticName, semanticsLen);
//            elementDesc.SemanticName = pTmp;
//            elementDesc.SemanticIndex = 0;
//            elementDesc.Format = format;
//            elementDesc.InputSlot = 0;
//            elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//            elementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
//            elementDesc.InstanceDataStepRate = 0;
//            reflectionData.mInputElementDescs.emplace_back(elementDesc);
//        }
//        // �C���v�b�g���C�A�E�g�̐���ݒ�
//        reflectionData.mNumInputElements = shaderDesc.InputParameters;
//        // �f�B�X�N���v�^�����W���擾
//        getDescriptorRange(pReflector, shaderDesc.BoundResources, D3D12_SHADER_VISIBILITY_VERTEX);
//    }
//    // ------------------------------- �s�N�Z���V�F�[�_�[ -------------------------------
//    if(pBlobPs_)
//    {
//        // �V�F�[�_�[����K�v�ȏ����擾����
//        Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector;
//        auto hr = D3DReflect(pBlobPs_->GetBufferPointer(), pBlobPs_->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
//        OrcaDebug::GraphicsLog("�s�N�Z���V�F�[�_�[�̃��t���N�V���������擾", hr);
//
//        // ���t���N�^�[��������擾
//        D3D12_SHADER_DESC shaderDesc;
//        pReflector->GetDesc(&shaderDesc);
//        // �f�B�X�N���v�^�����W���擾
//        getDescriptorRange(pReflector, shaderDesc.BoundResources, D3D12_SHADER_VISIBILITY_PIXEL);
//    }
//    // -------------------------------- �h���C���V�F�[�_ -------------------------------
//    if(pBlobDs_)
//    {
//        
//    }
//    // -------------------------------- �n���V�F�[�_�[ --------------------------------
//    if(pBlobHs_)
//    {
//        
//    }
//    // ------------------------------- �W�I���g���V�F�[�_�[ ------------------------------
//    if(pBlobGs_)
//    {
//        
//    }
//    return std::move(reflectionData);
//}
//
//
//// -------------------------- �V�F�[�_�[���t���N�V�����ŕԂ��p�̍\���̂̊֐� --------------------------
//OrcaGraphics::Shader::ShaderReflection::ReflectionData::ReflectionData(ReflectionData&& RHS_) noexcept
//{
//    *this = RHS_;
//    RHS_.mInputElementDescs.clear();
//}
//
//OrcaGraphics::Shader::ShaderReflection::ReflectionData::~ReflectionData()
//{
//    for (const auto& aaa : mInputElementDescs)
//    {
//        delete[] aaa.SemanticName;
//    }
//}
