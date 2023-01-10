#pragma once
#include<wrl.h>

// -------------------------------- 前方宣言 -------------------------------
struct ID3D12GraphicsCommandList;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Device;

// シェーダー基底クラス
namespace OrcaGraphics
{
    namespace Shader
    {
      

        class Shader
        {
        public:
            void Initialize(Microsoft::WRL::ComPtr<ID3D12Device> pDevice_, const char* VsPath_, const char* PsPath_);
            void StackGraphicsCmd(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> pCmdList_);
            
        private:
            Microsoft::WRL::ComPtr<ID3D12RootSignature> mpRootSignature{};
            Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPso{};
        };
    }
}
