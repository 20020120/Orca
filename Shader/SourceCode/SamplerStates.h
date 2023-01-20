#pragma once
#include <d3d12.h>
#include<vector>
#include<map>
#include<string>
namespace OrcaGraphics
{
    namespace Shader
    {
        // 公開先クラスの前方宣言
        class Shader;
    }


    namespace PipelineObject
    {
        class SamplerStates
        {
            // ---------------------------- 公開先のクラス ----------------------------
            friend class Shader::Shader;
            friend class RenderPipeline;

            // シェーダーリフレクションで取得した情報から静的サンプラーステートを生成して返す関数
            static std::vector<D3D12_STATIC_SAMPLER_DESC> GetStaticSamplerForReflection(const std::map<std::string, UINT>
                & reflectionData_);

            // スタティックサンプラーを取得
            static D3D12_STATIC_SAMPLER_DESC GetStaticSampler_Sample(); // サンプル用の静的サンプラー
            static D3D12_STATIC_SAMPLER_DESC GetStaticSampler_LinerClamp(UINT Slot_);  // リニアサンプラー
        };
    }
}
