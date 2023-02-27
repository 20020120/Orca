#pragma once
#include"ShaderDesc.h"
#include"GraphicsMacro.h"

#include<vector>
#include<map>
#include<d3d12shader.h>
#include<d3d12.h>

// ------------------------------------ 前方宣言 -----------------------------------
struct ID3D12GraphicsCommandList;
struct IDxcBlob;
struct IDxcUtils;
struct ID3D12RootSignature;
struct ID3D12PipelineState;

// -------------------------------- 描画用シェーダークラス --------------------------------
namespace Graphics
{
    namespace Shader
    {
        class Shader
        {
            // -------------------- バインドするためのデータや、リフレクションデータ -------------------
            struct Data
            {
                ~Data();
                void* m_pBin{};        // シェーダーの先頭アドレス
                size_t m_BinSize{};    // バイナリのサイズ
                void* m_pRef{};
                size_t m_RefSize{};
                D3D12_SHADER_DESC mShaderDesc{};
            };
            struct RootConstantInfo
            {
                D3D12_SHADER_BUFFER_DESC mShaderBufferDesc{};
                uint32_t mRegisterSpace{};
                uint32_t mBaseShaderRegister{};
            };

            using DescriptorRanges = std::map<std::string, D3D12_DESCRIPTOR_RANGE>; // ディスクリプタの省略型
            using SamplerInfo = std::map<std::string, UINT>;   // 使うサンプラーステートの情報を取得する(名前/バインドスロット)
            using RootConstants = std::map<std::string, RootConstantInfo>; // ルートコンスタントの情報を格納するバッファ
        public:
            // ------------------------------ 初期化 ------------------------------
            explicit Shader(IDxcUtils* pUtils_, const ShaderDesc& ShaderDesc_);
            ~Shader();
            // コマンドをセット
            void Set(ID3D12GraphicsCommandList* pCmdList_);

            void CreateRootRootSignature(IDxcUtils* pUtils_, const ShaderDesc& Desc_);   // ルートシグネチャを作成
        private:
            // --------------------------- 各シェーダーのデータ --------------------------
            Data m_Vs{};
            Data m_Ps{};
            ShaderType m_ShaderType; // 種類
            OrcaComPtr(ID3D12RootSignature) mpRootSignature {};
            OrcaComPtr(ID3D12PipelineState) mpPipelineState {};
        };
    }
}
