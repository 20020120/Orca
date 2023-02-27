#pragma once
// --------------------------------- どんなシェーダーか ---------------------------------
namespace Graphics
{
    namespace Shader
    {
        enum class ShaderType
        {
            Unknown,    // 未設定（初期値）
            Sample, // サンプル用
            Lambert, // ランバート
        };
    }
}
