#pragma once
#include"GraphicsMacro.h"
#include"ScreenConstants.h"
#include"GraphicsMinimum.h"

#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>
#include<DirectXMath.h>

// ------------------------------------ 前方宣言 -----------------------------------
namespace Graphics
{
    class DescriptorPool;
}
struct ID3D12CommandQueue;

// ---------------------------- Dx12のグラフィックスクラスの実装部分 ---------------------------
namespace Graphics
{
    class GraphicsImpl
    {
    public:
        GraphicsImpl() = default;
        ~GraphicsImpl() = default;

        void Initialize(HWND hWnd_);  // 初期化
        void Finalize(); // 終了処理

        void OpenCmdList() const;
        void CloseCmdList();
        void StackCmdList() const;

        [[nodiscard]] OrcaComPtr(ID3D12Device) GetDevice()const;
        [[nodiscard]] OrcaComPtr(ID3D12GraphicsCommandList) GetCmdList()const;
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
        OrcaComPtr(ID3D12Device) mpDevice{};                                       // デバイス
        OrcaComPtr(ID3D12CommandQueue)  mpCommandQueue{};                           // コマンドキュー
        OrcaComPtr(IDXGISwapChain3 ) mpSwapChain{};                                 // スワップチェーン
        OrcaComPtr(ID3D12CommandAllocator ) mpCommandAllocator[Orca::FrameCount]{}; // コマンドアロケーター
        OrcaComPtr(ID3D12GraphicsCommandList ) mpCommandList{};                     // コマンドリスト
        OrcaComPtr(ID3D12DescriptorHeap ) mpHeapRTV{};                              // ディスクリプタヒープのレンダーターゲット
        OrcaComPtr(ID3D12Resource) mpColorBuffer[Orca::FrameCount]{};              // カラーバッファ
        D3D12_CPU_DESCRIPTOR_HANDLE mHandleRTV[Orca::FrameCount]{};                            // レンダーターゲットビュー
        OrcaComPtr(ID3D12Fence) mpFence {};                                         // フェンスオブジェクト


    private:
        void Present(uint32_t Interval_);   // 画面に表示し、次のフレームの準備をする
        void WaitGpu();                     // GPUの実行を待つ
        void AddDebugFlag() const;                // デバッグフラグを追加する

        HANDLE mFenceEvent{};
        UINT mFrameIndex{};
        unsigned long long mFenceCounter[Orca::FrameCount]{};

    private:
        bool CreateDescriptorPool();
        void CreateViewport(); // ビューポートを作成する
        void CreateScissor(); // シザー矩形を作成する

        Microsoft::WRL::ComPtr<ID3D12Resource> mpDepthBuffer{};
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpHeapDSV{};
        D3D12_CPU_DESCRIPTOR_HANDLE mHandleDsV{};

        DescriptorPool* mpPool[POOL_COUNT]{};         // ディスクリプタプールです.`
    private:
        float mRotateAngle{};
        D3D12_RECT mScissor{};
        D3D12_VIEWPORT mViewPort{};
    };
}
