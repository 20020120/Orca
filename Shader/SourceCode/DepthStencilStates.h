#pragma once
#include<d3d12.h>
namespace OrcaGraphics
{
    namespace Shader
    {
        // 公開先クラスの前方宣言
        class Shader;
    }

    namespace PipelineObject
    {
        class DepthStencilStates
        {
            // ---------------------------- 公開先のクラス ----------------------------
            friend class Shader::Shader;

            static D3D12_DEPTH_STENCIL_DESC GetSampleDepthStencilState();    // サンプル用のデプスステンシルステートを取得
           
        };

       
      
    }
}
