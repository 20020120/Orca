#include"pch.h"
#include"GraphicsImpl.h"
#include"GraphicsLogger.h"
#include<dxgi1_4.h>
#include<d3d12.h>
#include<d3dcompiler.h>
#include"ResourceUploadBatch.h"
#include"DescriptorPool.h"

#include"ScreenConstants.h"
#include"OrcaException.h"

void Graphics::GraphicsImpl::Initialize(HWND hWnd_)
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
    CreateDescriptorPool();

    CreateDepthBuffer();

    CreateViewport();
    CreateScissor();
}

void Graphics::GraphicsImpl::Finalize()
{
    // ---------------------------------- 終了処理 ---------------------------------
    WaitGpu();

    for (auto p : mpPool)
    {
        if (p)
        {
            delete p;
            p = nullptr;
        }
    }
}

void Graphics::GraphicsImpl::OpenCmdList() const
{
    // ---------------------------------- 描画処理 ---------------------------------

    // コマンドの記録を開始
    mpCommandAllocator[mFrameIndex]->Reset();
    mpCommandList->Reset(mpCommandAllocator[mFrameIndex].Get(), nullptr);

    // リソースバリアの設定
    D3D12_RESOURCE_BARRIER barrier;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = mpColorBuffer[mFrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    mpCommandList->ResourceBarrier(1, &barrier);
    mpCommandList->OMSetRenderTargets(1, &mHandleRTV[mFrameIndex], FALSE, &mHandleDsV);

    // クリアカラーの設定
    constexpr  float clearColor[] = { 0.3f,0.3f,0.3f,1.0f };
    // レンダーターゲットをクリア
    mpCommandList->ClearRenderTargetView(mHandleRTV[mFrameIndex], clearColor, 0, nullptr);
    mpCommandList->ClearDepthStencilView(mHandleDsV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void Graphics::GraphicsImpl::CloseCmdList()
{
    D3D12_RESOURCE_BARRIER barrier;
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

void Graphics::GraphicsImpl::StackCmdList() const
{
    // 描画処理
    {
        ID3D12DescriptorHeap* const pHeaps[] = {
          mpPool[POOL_TYPE_RES]->GetHeap()
        };

        mpCommandList->SetDescriptorHeaps(1, pHeaps);
        mpCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        mpCommandList->RSSetViewports(1, &mViewPort);
        mpCommandList->RSSetScissorRects(1, &mScissor);
    }
}


Microsoft::WRL::ComPtr<ID3D12Device> Graphics::GraphicsImpl::GetDevice() const
{
    return mpDevice;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Graphics::GraphicsImpl::GetCmdList() const
{
    return mpCommandList;
}

Graphics::DescriptorPool* Graphics::GraphicsImpl::GetDescriptorPool(POOL_TYPE Type_) const
{
    return mpPool[Type_];
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> Graphics::GraphicsImpl::GetCommandQueue() const
{
    return mpCommandQueue;
}

void Graphics::GraphicsImpl::CreateDevice()
{
    // デバイスの初期化
    const auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(mpDevice.ReleaseAndGetAddressOf()));

    OrcaDebug::GraphicsLog("デバイスを初期化", hr);
}

void Graphics::GraphicsImpl::CreateCommandQueue()
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

void Graphics::GraphicsImpl::CreateSwapChain(HWND hWnd_)
{
    IDXGIFactory4* pFactory{};
    auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
    OrcaDebug::GraphicsLog("IDXGIFactory4を初期化", hr);

    // スワップチェーンの初期化
    DXGI_SWAP_CHAIN_DESC desc;
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

void Graphics::GraphicsImpl::CreateCommandAllocator()
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

void Graphics::GraphicsImpl::CreateCommandList()
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

void Graphics::GraphicsImpl::CreateRenderTargetView()
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
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

void Graphics::GraphicsImpl::CreateFence()
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
    Orca_NullException(mFenceEvent)

}

void Graphics::GraphicsImpl::CreateDepthBuffer()
{
    // ヒーププロパティを設定
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_DEFAULT;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // リソースの設定
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Alignment = 0;
    desc.Width = Orca::ScreenWidth;
    desc.Height = Orca::ScreenHeight;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_D32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearValue{};
    clearValue.Format = DXGI_FORMAT_D32_FLOAT;
    clearValue.DepthStencil.Depth = 1.0;
    clearValue.DepthStencil.Stencil = 0;

    auto hr = mpDevice->CreateCommittedResource(
        &prp,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clearValue,
        IID_PPV_ARGS(mpDepthBuffer.GetAddressOf())
    );
    OrcaDebug::GraphicsLog("デプスバッファを作成", hr);

    // ディスクリプタヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC descDH{};
    descDH.NumDescriptors = 1;
    descDH.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    descDH.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    descDH.NodeMask = 0;
    hr = mpDevice->CreateDescriptorHeap(
        &descDH,
        IID_PPV_ARGS(mpHeapDSV.ReleaseAndGetAddressOf())
    );
    OrcaDebug::GraphicsLog("ディスクリプタヒープを作成", hr);

    const auto handle = mpHeapDSV->GetCPUDescriptorHandleForHeapStart();
    auto incrementSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc{};
    viewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipSlice = 0;
    viewDesc.Flags = D3D12_DSV_FLAG_NONE;

    mpDevice->CreateDepthStencilView(mpDepthBuffer.Get(), &viewDesc, handle);
    mHandleDsV = handle;
}

void Graphics::GraphicsImpl::Present(uint32_t Interval_)
{
    // 画面に表示
    mpSwapChain->Present(Interval_, 0);

    // シグナル処理
    const auto currentValue = mFenceCounter[mFrameIndex];
    mpCommandQueue->Signal(mpFence.Get(), currentValue);

    // バックバッファ番号を更新
    mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();

    // 次のフレームの描画準備がまだであれば待機する
    if (mpFence->GetCompletedValue() < mFenceCounter[mFrameIndex])
    {
        mpFence->SetEventOnCompletion(mFenceCounter[mFrameIndex], mFenceEvent);
        WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);
    }

    // 次のフレームのカウンターを増やす
    mFenceCounter[mFrameIndex] = currentValue + 1;
}

void Graphics::GraphicsImpl::WaitGpu()
{
    Orca_NullException(mpCommandQueue)
        Orca_NullException(mpFence)
        Orca_NullException(mFenceEvent)

        // シグナル処理
        mpCommandQueue->Signal(mpFence.Get(), mFenceCounter[mFrameIndex]);
    // 完了時にイベントを設定する
    mpFence->SetEventOnCompletion(mFenceCounter[mFrameIndex], mFenceEvent);
    // 待機処理
    WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);

    // カウンターを増やす
    mFenceCounter[mFrameIndex]++;

}

void Graphics::GraphicsImpl::AddDebugFlag() const
{
    // デバッグモードのみこの関数を有効にする
#if defined(DEBUG)||defined(_DEBUG)
    Microsoft::WRL::ComPtr<ID3D12Debug> pDebug{};
    const auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(pDebug.GetAddressOf()));

    // デバッグレイヤーを有効化
    if (SUCCEEDED(hr))
    {
        pDebug->EnableDebugLayer();
    }
#endif
}


bool Graphics::GraphicsImpl::CreateDescriptorPool()
{
    // ディスクリプタプールの生成.
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};

        desc.NodeMask = 1;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 512;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (!DescriptorPool::Create(mpDevice.Get(), &desc, &mpPool[POOL_TYPE_RES]))
        {
            return false;
        }

        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        desc.NumDescriptors = 256;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (!DescriptorPool::Create(mpDevice.Get(), &desc, &mpPool[POOL_TYPE_SMP]))
        {
            return false;
        }

        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = 512;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        if (!DescriptorPool::Create(mpDevice.Get(), &desc, &mpPool[POOL_TYPE_RTV]))
        {
            return false;
        }

        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        desc.NumDescriptors = 512;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        if (!DescriptorPool::Create(mpDevice.Get(), &desc, &mpPool[POOL_TYPE_DSV]))
        {
            return false;
        }
    }
    return true;
}

void Graphics::GraphicsImpl::CreateViewport()
{
    // ビューポートを作成する
    mViewPort.TopLeftX = 0.0f;
    mViewPort.TopLeftY = 0.0f;
    mViewPort.Width = Orca::ScreenWidth;
    mViewPort.Height = Orca::ScreenHeight;
    mViewPort.MinDepth = 0.0f;
    mViewPort.MaxDepth = 1.0f;
}

void Graphics::GraphicsImpl::CreateScissor()
{
    // シザー矩形を作成
    mScissor.left = 0;
    mScissor.right = Orca::ScreenWidth;
    mScissor.top = 0;
    mScissor.bottom = Orca::ScreenHeight;
}
