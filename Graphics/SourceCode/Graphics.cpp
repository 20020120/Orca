#include"pch.h"
#include"Graphics.h"
#include"GraphicsLogger.h"
#include<dxgi1_4.h>
#include<d3d12.h>
#include"ScreenConstants.h"
#include"OrcaException.h"
OrcaGraphics::Graphics::Graphics()= default;
OrcaGraphics::Graphics::~Graphics()
{
    Finalize();
}


void OrcaGraphics::Graphics::Initialize(HWND hWnd_)
{
    AddDebugFlag();

    // ----------------------------- DirectX12の初期化 -----------------------------
    CreateDevice();
    CreateCommandQueue();
    CreateSwapChain(hWnd_);
    CreateCommandAllocator();
    CreateCommandList();
    CreateRenderTargetView();
    CreateFence();
}

void OrcaGraphics::Graphics::Finalize()
{
    // ---------------------------------- 終了処理 ---------------------------------
    WaitGpu();
}

void OrcaGraphics::Graphics::Render()
{
    // ---------------------------------- 描画処理 ---------------------------------

    // コマンドの記録を開始
    mpCommandAllocator[mFrameIndex]->Reset();
    mpCommandList->Reset(mpCommandAllocator[mFrameIndex].Get(), nullptr);

    // リソースバリアの設定
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = mpColorBuffer[mFrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    mpCommandList->ResourceBarrier(1, &barrier);
    mpCommandList->OMSetRenderTargets(1, &mHandleRTV[mFrameIndex], FALSE, nullptr);

    // クリアカラーの設定
    constexpr  float clearColor[] = { 0.0f,0.0f,1.0f,1.0f };
    // レンダーターゲットをクリア
    mpCommandList->ClearRenderTargetView(mHandleRTV[mFrameIndex], clearColor, 0, nullptr);

    // 描画処理
    {
        
    }

    // リソースバリアの設定
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = mpColorBuffer[mFrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    // リソースバリア
    mpCommandList->ResourceBarrier(1, &barrier);

    // コマンドの記録を終了
    mpCommandList->Close();

    // コマンドを実行
    ID3D12CommandList* ppCmdList[]{ mpCommandList.Get() };
    mpCommandQueue->ExecuteCommandLists(1, ppCmdList);

    // 画面に表示
    Present(1);
}

void OrcaGraphics::Graphics::CreateDevice()
{
    // デバイスの初期化
    const auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(mpDevice.ReleaseAndGetAddressOf()));

    OrcaDebug::GraphicsLog("デバイスを初期化", hr);
}

void OrcaGraphics::Graphics::CreateCommandQueue()
{
    // ------------------------------- コマンドキューの作成 ------------------------------

    D3D12_COMMAND_QUEUE_DESC desc;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    const auto hr = mpDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(mpCommandQueue.ReleaseAndGetAddressOf()));
    OrcaDebug::GraphicsLog("コマンドキューを初期化", hr);

}

void OrcaGraphics::Graphics::CreateSwapChain(HWND hWnd_)
{
    IDXGIFactory4* pFactory{};
    auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
    OrcaDebug::GraphicsLog("IDXGIFactory4を初期化", hr);

    // スワップチェーンの初期化
    DXGI_SWAP_CHAIN_DESC desc{};
    desc.BufferDesc.Width = Orca::ScreenWidth;
    desc.BufferDesc.Height = Orca::ScreenHeight;
    desc.BufferDesc.RefreshRate.Numerator = Orca::RefreshRate;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = Orca::FrameCount;
    desc.OutputWindow = hWnd_;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // スワップチェーンを作成
    IDXGISwapChain* pSwapChain{};
    hr = pFactory->CreateSwapChain(mpCommandQueue.Get(), &desc, &pSwapChain);
    OrcaDebug::GraphicsLog("スワップチェーンを作成", hr);

    // スワップチェーンを取得
    hr = pSwapChain->QueryInterface(IID_PPV_ARGS(mpSwapChain.ReleaseAndGetAddressOf()));
    OrcaDebug::GraphicsLog("スワップチェーンを取得", hr);

    // バックバッファの番号を取得
    mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();

    // 要素を解放
    pFactory->Release();
    pSwapChain->Release();

}

void OrcaGraphics::Graphics::CreateCommandAllocator()
{
    HRESULT hr{};
    for (int i = 0; i < Orca::FrameCount; ++i)
    {
        hr = mpDevice->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(mpCommandAllocator[i].ReleaseAndGetAddressOf())
        );
        OrcaDebug::GraphicsLog("コマンドアロケーターを初期化", hr);
    }
}

void OrcaGraphics::Graphics::CreateCommandList()
{
    const auto hr = mpDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        mpCommandAllocator[mFrameIndex].Get(),
        nullptr,
        IID_PPV_ARGS(mpCommandList.ReleaseAndGetAddressOf())
    );

    OrcaDebug::GraphicsLog("コマンドリストを作成", hr);
}

void OrcaGraphics::Graphics::CreateRenderTargetView()
{
    // ディスクリプタヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.NumDescriptors = Orca::FrameCount;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask = 0;

    // ディスクリプタヒープの作成
    auto hr = mpDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(mpHeapRTV.ReleaseAndGetAddressOf()));
    OrcaDebug::GraphicsLog("ディスクリプタヒープの作成", hr);


    auto handle = mpHeapRTV->GetCPUDescriptorHandleForHeapStart();
    // このサイズはGPUに依存しているので固定値は使っていけない
    const auto incrementSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // レンダーターゲットビューの設定
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    rtvDesc.Texture2D.PlaneSlice = 0;

    for (int i = 0; i < Orca::FrameCount; ++i)
    {
        hr = mpSwapChain->GetBuffer(i, IID_PPV_ARGS(mpColorBuffer[i].ReleaseAndGetAddressOf()));
        OrcaDebug::GraphicsLog("バッファを取得", hr);

        mpDevice->CreateRenderTargetView(mpColorBuffer[i].Get(), &rtvDesc, handle);
        mHandleRTV[i] = handle;
        handle.ptr += incrementSize;
    }


}

void OrcaGraphics::Graphics::CreateFence()
{
    // フェンスオブジェクトを作成
    const auto hr = mpDevice->CreateFence(
        mFenceCounter[mFrameIndex],
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(&mpFence)
    );

    OrcaDebug::GraphicsLog("フェンスオブジェクトを生成", hr);

    // フェンスイベントを作成
    mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    Orca_NullExeption(mFenceEvent)

}

void OrcaGraphics::Graphics::Present(uint32_t Interval_)
{
    // 画面に表示
    mpSwapChain->Present(Interval_, 0);

    // シグナル処理
    const auto currentValue = mFenceCounter[mFrameIndex];
    mpCommandQueue->Signal(mpFence.Get(), currentValue);

    // バックバッファ番号を更新
    mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();

    // 次のフレームの描画準備がまだであれば待機する
    if(mpFence->GetCompletedValue()<mFenceCounter[mFrameIndex])
    {
        mpFence->SetEventOnCompletion(mFenceCounter[mFrameIndex], mFenceEvent);
        WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);
    }

    // 次のフレームのカウンターを増やす
    mFenceCounter[mFrameIndex] = currentValue + 1;
}

void OrcaGraphics::Graphics::WaitGpu()
{
    Orca_NullExeption(mpCommandQueue)
    Orca_NullExeption(mpFence)
    Orca_NullExeption(mFenceEvent)

    // シグナル処理
    mpCommandQueue->Signal(mpFence.Get(), mFenceCounter[mFrameIndex]);
    // 完了時にイベントを設定する
    mpFence->SetEventOnCompletion(mFenceCounter[mFrameIndex], mFenceEvent);
    // 待機処理
    WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);

    // カウンターを増やす
    mFenceCounter[mFrameIndex]++;

}

void OrcaGraphics::Graphics::AddDebugFlag() const
{
    // デバッグモードのみこの関数を有効にする
#if defined(DEBUG)||defined(_DEBUG)
    Microsoft::WRL::ComPtr<ID3D12Debug> pDebug{};
    const auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(pDebug.GetAddressOf()));

    // デバッグレイヤーを有効化
    if(SUCCEEDED(hr))
    {
        pDebug->EnableDebugLayer();
    }
#endif
}
