#pragma once
#include <cstdint>
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>
#include<DirectXMath.h>

#include "ConstantBuffer.h"
#include"ScreenConstants.h"
#include "Texture.h"


namespace OrcaGraphics
{
    // DirectX12の描画クラス
    class Graphics
    {
    public:
        enum POOL_TYPE
        {
            POOL_TYPE_RES = 0,     // CBV / SRV / UAV
            POOL_TYPE_SMP = 1,     // Sampler
            POOL_TYPE_RTV = 2,     // RTV
            POOL_TYPE_DSV = 3,     // DSV
            POOL_COUNT = 4,
        };
    public:
        Graphics();
        ~Graphics();

        void Initialize(HWND hWnd_);  // 初期化
        void Finalize(); // 終了処理
        void OnTerm();  // 終了時の処理

        void OpenCmdList();
        void CloseCmdList();

        void StackCmdList();

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Device> GetDevice()const;
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCmdList()const;
        [[nodiscard]] DescriptorPool* GetDescriptorPool(POOL_TYPE Type_)const;
        [[nodiscard]] OrcaComPtr(ID3D12CommandQueue) GetCommandQueue()const;
    private:
        void CreateDevice();                  // デバイスの初期化
        void CreateCommandQueue();            // コマンドキューの初期化
        void CreateSwapChain(HWND hWnd_);     // スワップチェーンの作成
        void CreateCommandAllocator();        // コマンドアロケーターを作成
        void CreateCommandList();             // コマンドリストを作成
        void CreateRenderTargetView();        // レンダーターゲットビューを作成
        void CreateFence();                   // フェンスオブジェクトを作成する
        void CreateDepthBuffer();             // デプスバッファを作成

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


    private:
        void Present(uint32_t Interval_);   // 画面に表示し、次のフレームの準備をする
        void WaitGpu();                     // GPUの実行を待つ
        void AddDebugFlag() const;                // デバッグフラグを追加する

        HANDLE mFenceEvent{}; 
        UINT mFrameIndex{};
        unsigned long long mFenceCounter[Orca::FrameCount]{};

    private:
        struct Vertex
        {
            DirectX::XMFLOAT3 Position{};
            DirectX::XMFLOAT2 TexCoord{};
        };

        struct alignas(256)CB_Simple
        {
            DirectX::XMMATRIX World{};
            DirectX::XMMATRIX ViewMat{};
            DirectX::XMMATRIX ProjMat{};
        };


        bool CreateDescriptorPool();
        void CreateViewport(); // ビューポートを作成する
        void CreateScissor(); // シザー矩形を作成する


        Microsoft::WRL::ComPtr<ID3D12Resource> mpDepthBuffer{};
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpHeapDSV{};
        D3D12_CPU_DESCRIPTOR_HANDLE mHandleDsV{};

        DescriptorPool* mpPool[POOL_COUNT];         // ディスクリプタプールです.

    private:
        float mRotateAngle{};
        D3D12_RECT mScissor{};
        D3D12_VIEWPORT mViewPort{};
    };
}
