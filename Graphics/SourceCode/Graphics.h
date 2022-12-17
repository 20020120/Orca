#pragma once
#include <cstdint>
#include<d3d12.h>
#include<wrl.h>
#include<dxgi1_4.h>
#include<DirectXMath.h>
#include"ScreenConstants.h"
namespace OrcaGraphics
{

    // ---------------------------------- 前方宣言 ---------------------------------



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
            DirectX::XMFLOAT4 Color{};
        };

        struct alignas(256)CB_Simple
        {
            DirectX::XMMATRIX World{};
            DirectX::XMMATRIX ViewMat{};
            DirectX::XMMATRIX ProjMat{};
        };
        template<class T>
        struct ConstantBufferView
        {
            D3D12_CONSTANT_BUFFER_VIEW_DESC mDesc{};
            D3D12_CPU_DESCRIPTOR_HANDLE mHandleCPU{};
            D3D12_GPU_DESCRIPTOR_HANDLE mHandleGPU{};
            T* mpBuffer{};
        };

        void CreateVertexBuffer(); // 頂点バッファを作成する
        void CreateConstantBuffer(); // 定数バッファを作成する

        Microsoft::WRL::ComPtr<ID3D12Resource> mpVertexBuffer{};
        D3D12_VERTEX_BUFFER_VIEW mVbView{};

        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpHeapCbV{};
        Microsoft::WRL::ComPtr<ID3D12Resource> mpConstantBuffer[Orca::FrameCount]{};
        ConstantBufferView<CB_Simple> mCbV[Orca::FrameCount]{};

        D3D12_VIEWPORT mViewPort{};
    };
}