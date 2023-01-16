#include"SamplerStates.h"
#include"SamplerStateNames.h"
#include"OrcaException.h"

std::vector<D3D12_STATIC_SAMPLER_DESC> OrcaGraphics::PipelineObject::SamplerStates::GetStaticSamplerForReflection(
    const std::map<std::string, UINT>& reflectionData_)
{
    std::vector<D3D12_STATIC_SAMPLER_DESC> samplers{};
    samplers.reserve(reflectionData_.size());
    for (const auto& [name, slot] : reflectionData_)
    {
        if (std::strcmp(name.c_str(), Str(SS_LINER_CLAMP)) != 0)
        {
            samplers.emplace_back(GetStaticSampler_LinerClamp(slot));
            continue;
        }
        Orca_Assert("未実装のサンプラーステートを使用しようとしています");
    }
    return samplers;
}

D3D12_STATIC_SAMPLER_DESC OrcaGraphics::PipelineObject::SamplerStates::GetStaticSampler_Sample()
{
    // スタティックサンプラーの設定
    D3D12_STATIC_SAMPLER_DESC descSSD;
    descSSD.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    descSSD.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
    descSSD.MaxAnisotropy = 1;
    descSSD.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    descSSD.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    descSSD.MinLOD = -D3D12_FLOAT32_MAX;
    descSSD.MaxLOD = +D3D12_FLOAT32_MAX;
    descSSD.ShaderRegister = 0;
    descSSD.RegisterSpace = 0;
    descSSD.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    return descSSD;
}

D3D12_STATIC_SAMPLER_DESC OrcaGraphics::PipelineObject::SamplerStates::GetStaticSampler_LinerClamp(UINT Slot_)
{
    // スタティックサンプラーの設定
    D3D12_STATIC_SAMPLER_DESC descSSD;
    descSSD.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    descSSD.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    descSSD.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
    descSSD.MaxAnisotropy = 1;
    descSSD.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    descSSD.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    descSSD.MinLOD = -D3D12_FLOAT32_MAX;
    descSSD.MaxLOD = +D3D12_FLOAT32_MAX;
    descSSD.ShaderRegister = Slot_;
    descSSD.RegisterSpace = 0;
    descSSD.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    return descSSD;
}
