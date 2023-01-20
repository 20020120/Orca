#include"InputLayout.h"
#include "ReflectionHelpers.h"

#include <d3d12shader.h>
std::vector<D3D12_INPUT_ELEMENT_DESC> OrcaGraphics::InputLayout::InputLayout::CreateInputElementDesc(
    const Shader::ShaderBuilder& ShaderBuilder_)
{
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs{};

    // ------------------------------ リフレクション情報を取得 -----------------------------
    const auto pReflector = ShaderBuilder_.GetReflector();
    const D3D12_SHADER_DESC shaderDesc = ShaderBuilder_.GetShaderDesc();
    inputElementDescs.reserve(shaderDesc.InputParameters);

    for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i) {
        D3D12_SIGNATURE_PARAMETER_DESC sigDesc;
        pReflector->GetInputParameterDesc(i, &sigDesc);

        //フォーマットを取得
        const auto format = Shader::ReflectionHelpers::GetDxgiFormat(sigDesc.ComponentType, sigDesc.Mask);
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
        inputElementDescs.emplace_back(elementDesc);
    }

    return inputElementDescs;
}
