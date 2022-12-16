#pragma once
#include <cstdint>
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>
#include"ScreenConstants.h"
namespace OrcaGraphics
{
    // DirectX12の描画クラス
    class Graphics
    {
    public:
        Graphics();
        ~Graphics();

        void Initialize(HWND hWnd_);  // 初期化
        void Finalize(); // 終了処理

        void Render();  // 描画処理

    private:
        void CreateDevice();                  // デバイスの初期化
        void CreateCommandQueue();            // コマンドキューの初期化
        void CreateSwapChain(HWND hWnd_);     // スワップチェーンの作成
        void CreateCommandAllocator();        // コマンドアロケーターを作成
        void CreateCommandList();             // コマンドリストを作成
        void CreateRenderTargetView();        // レンダーターゲットビューを作成
        void CreateFence();                   // フェンスオブジェクトを作成する

        void Present(uint32_t Interval_);   // 画面に表示し、次のフレームの準備をする

    private:
        // ----------------------------------- 変数 ----------------------------------
        Microsoft::WRL::ComPtr<ID3D12Device> mpDevice{};                                       // デバイス
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue{};                           // コマンドキュー
        Microsoft::WRL::ComPtr<IDXGISwapChain3> mpSwapChain{};                                 // スワップチェーン
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mpCommandAllocator[Orca::FrameCount]{}; // コマンドアロケーター
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mpCommandList{};                     // コマンドリスト
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpHeapRTV{};                              // ディスクリプタヒープのレンダーターゲット
        Microsoft::WRL::ComPtr<ID3D12Resource> mpColorBuffer[Orca::FrameCount]{};              // カラーバッファ
        D3D12_CPU_DESCRIPTOR_HANDLE mHandleRTV[Orca::FrameCount]{};                            // レンダーターゲットビュー
        Microsoft::WRL::ComPtr<ID3D12Fence> mpFence{};                                         // フェンスオブジェクト

        HANDLE mFenceEvent{}; 
        UINT mFrameIndex{};
        unsigned long long mFenceCounter[Orca::FrameCount]{};
    };
}