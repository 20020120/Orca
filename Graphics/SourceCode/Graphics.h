#pragma once
#include<d3d12.h>
#include<wrl.h>


namespace OrcaGraphics
{
    // DirectX12の描画クラス
    class Graphics
    {
    public:
        Graphics();
        ~Graphics();

        void Initialize();  // 初期化


    private:
        void CreateDevice();        // デバイスの初期化
        void CreateCommandQueue();  // コマンドキューの初期化
        void CreateSwapChain();     // スワップチェーンの作成
    private:
        // ----------------------------------- 変数 ----------------------------------
        Microsoft::WRL::ComPtr<ID3D12Device> mpDevice{};             // デバイス
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue{}; // コマンドキュー


    };
}