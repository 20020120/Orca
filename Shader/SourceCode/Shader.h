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
namespace OrcaGraphics
{
    namespace Shader
    {
        class Shader
        {
            // -------------------- バインドするためのデータや、リフレクションデータ -------------------
            struct Data
            {
                void* m_pBin{};        // シェーダーの先頭アドレス
                size_t m_BinSize{};    // バイナリのサイズ
                void* m_pRef{};
                size_t m_RefSize{};
                D3D12_SHADER_DESC mShaderDesc{};
            };
            
            using DescriptorRanges = std::map<std::string, D3D12_DESCRIPTOR_RANGE>; // ディスクリプタの省略型
            using SamplerInfo = std::map<std::string, UINT>;   // 使うサンプラーステートの情報を取得する(名前/バインドスロット)
        public:
            // ------------------------------ 初期化 ------------------------------
            explicit Shader(IDxcUtils* pUtils_, const ShaderDesc& ShaderDesc_);

            // コマンドをセット
            void Set(ID3D12GraphicsCommandList* pCmdList_);

            void CreateRootRootSignature(IDxcUtils* pUtils_, const ShaderDesc& Desc_);   // ルートシグネチャを作成
            void CreatePSO(const std::vector<D3D12_INPUT_ELEMENT_DESC>& InputElementDescs_, 
                const ShaderDesc& ShaderDesc_);   // パイプラインステートオブジェクトを作成
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
