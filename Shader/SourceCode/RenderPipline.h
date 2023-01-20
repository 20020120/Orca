#pragma once
// 描画時に描画パイプラインの情報をバインド＆保持するクラス
#include"GraphicsMacro.h"
#include"ShaderDesc.h"
#include"ShaderBuilder.h"

#include<map>
#include<tuple>
#include<d3d12.h>
#include<vector>
// ------------------------------------ 前方宣言 -----------------------------------
struct ID3D12GraphicsCommandList;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Device;

namespace OrcaGraphics
{
    class RenderPipeline
    {
    public:
        // -------------------------------- 初期化 --------------------------------
        RenderPipeline(OrcaComPtr(ID3D12Device) pDevice_, const Shader::ShaderDesc& ShaderDesc_);

        RenderPipeline() = delete;
        ~RenderPipeline() = default;
        void StackGraphicsCmd(OrcaComPtr(ID3D12GraphicsCommandList) pCmdList_);
    private:
        OrcaComPtr(ID3D12RootSignature) mpRootSignature {};
        OrcaComPtr(ID3D12PipelineState) mpPipelineState {};
        Shader::ShaderDesc mShaderDesc{};

    private:

        // --------------------------- ディスクリプタレンジを作成 ---------------------------
        using DescriptorRanges = std::multimap<std::string, std::tuple<Shader::ShaderStage, D3D12_DESCRIPTOR_RANGE>>;
        using SamplerInfo = std::map<std::string, UINT>;   // 使うサンプラーステートの情報を取得する(名前/バインドスロット)

        void CreateRootParameter(const Shader::ShaderBuilder& ShaderBuilder_, std::vector<D3D12_ROOT_PARAMETER>& RootParameters_,
            DescriptorRanges& DescriptorRanges_, SamplerInfo& SamplersInfo_) const;
        static void CreateDescriptorRange(const Shader::ShaderBuilder& ShaderBuilder_, DescriptorRanges& DescriptorRanges_, SamplerInfo& SamplerInfo_);
    };
}
