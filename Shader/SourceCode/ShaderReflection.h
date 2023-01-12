#pragma once
#include <d3d12.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include<vector>
#include<wrl.h>

namespace OrcaGraphics
{
    namespace Shader
    {
        // シェーダーリフレクション用クラス(シェーダー系のクラス以外には隠しておくこと)
        class ShaderReflection
        {
            // ----------------------------- 公開先クラス ----------------------------
            friend class Shader;

            enum   ShaderStage
            {
                Vertex,
                Domain,
                Hull,
                Geometry,
                Pixel,
                Compute
            };

            // -------------------- シェーダーからリフレクション情報を返す構造体 -------------------
            struct ReflectionData
            {
                ReflectionData() = default;
                ReflectionData(const ReflectionData& RHS_) = default;
                ReflectionData& operator=(const ReflectionData&) = default;
                ReflectionData(ReflectionData&& RHS_)noexcept;
                ~ReflectionData();

                std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElementDescs{};   // インプットレイアウト
                UINT mNumInputElements{};   // インプットレイアウトの数
                std::vector<D3D12_DESCRIPTOR_RANGE> mDescriptorRanges{};    // ディスクリプタレンジ
                std::vector<ShaderStage> mDescriptorRangeShaderStage{};  // ディスクリプタレンジのシェーダーステージ

            };

            
            // シェーダーリフレクションを実行する
            static ReflectionData Execute(Microsoft::WRL::ComPtr<ID3DBlob> pBlobVs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobPs_,
                Microsoft::WRL::ComPtr<ID3DBlob> pBlobGs_ = nullptr, Microsoft::WRL::ComPtr<ID3DBlob> pBlobDs_ = nullptr,
                Microsoft::WRL::ComPtr<ID3DBlob> pBlobHs_ = nullptr);
        };
    }
}
