#pragma once
#include <d3d12.h>
#include <d3dcommon.h>
#include<vector>
#include<tuple>
#include<wrl.h>
#include<map>
#include<string>

namespace OrcaGraphics
{
    namespace Shader
    {
        // シェーダーリフレクション用クラス(シェーダー系のクラス以外には隠しておくこと)
        class ShaderReflection
        {
            // ----------------------------- 公開先クラス ----------------------------
            friend class Shader;

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
                std::multimap<std::string, std::tuple<D3D12_SHADER_VISIBILITY, D3D12_DESCRIPTOR_RANGE>> mDescriptorRanges{};// ディスクリプタレンジ
                std::map<std::string, UINT> SamplerStatesInfo{};   // 使うサンプラーステートの情報を取得する(名前/バインドスロット)
            };

            
            // シェーダーリフレクションを実行する
            static ReflectionData Execute(Microsoft::WRL::ComPtr<ID3DBlob> pBlobVs_, Microsoft::WRL::ComPtr<ID3DBlob> pBlobPs_,
                Microsoft::WRL::ComPtr<ID3DBlob> pBlobGs_ = nullptr, Microsoft::WRL::ComPtr<ID3DBlob> pBlobDs_ = nullptr,
                Microsoft::WRL::ComPtr<ID3DBlob> pBlobHs_ = nullptr);
        };
    }
}
