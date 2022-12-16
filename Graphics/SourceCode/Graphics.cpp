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

    // ----------------------------- DirectX12�̏����� -----------------------------
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
    // ---------------------------------- �I������ ---------------------------------
    WaitGpu();
}

void OrcaGraphics::Graphics::Render()
{
    // ---------------------------------- �`�揈�� ---------------------------------

    // �R�}���h�̋L�^���J�n
    mpCommandAllocator[mFrameIndex]->Reset();
    mpCommandList->Reset(mpCommandAllocator[mFrameIndex].Get(), nullptr);

    // ���\�[�X�o���A�̐ݒ�
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = mpColorBuffer[mFrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    mpCommandList->ResourceBarrier(1, &barrier);
    mpCommandList->OMSetRenderTargets(1, &mHandleRTV[mFrameIndex], FALSE, nullptr);

    // �N���A�J���[�̐ݒ�
    constexpr  float clearColor[] = { 0.0f,0.0f,1.0f,1.0f };
    // �����_�[�^�[�Q�b�g���N���A
    mpCommandList->ClearRenderTargetView(mHandleRTV[mFrameIndex], clearColor, 0, nullptr);

    // �`�揈��
    {
        
    }

    // ���\�[�X�o���A�̐ݒ�
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = mpColorBuffer[mFrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    // ���\�[�X�o���A
    mpCommandList->ResourceBarrier(1, &barrier);

    // �R�}���h�̋L�^���I��
    mpCommandList->Close();

    // �R�}���h�����s
    ID3D12CommandList* ppCmdList[]{ mpCommandList.Get() };
    mpCommandQueue->ExecuteCommandLists(1, ppCmdList);

    // ��ʂɕ\��
    Present(1);
}

void OrcaGraphics::Graphics::CreateDevice()
{
    // �f�o�C�X�̏�����
    const auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(mpDevice.ReleaseAndGetAddressOf()));

    OrcaDebug::GraphicsLog("�f�o�C�X��������", hr);
}

void OrcaGraphics::Graphics::CreateCommandQueue()
{
    // ------------------------------- �R�}���h�L���[�̍쐬 ------------------------------

    D3D12_COMMAND_QUEUE_DESC desc;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    const auto hr = mpDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(mpCommandQueue.ReleaseAndGetAddressOf()));
    OrcaDebug::GraphicsLog("�R�}���h�L���[��������", hr);

}

void OrcaGraphics::Graphics::CreateSwapChain(HWND hWnd_)
{
    IDXGIFactory4* pFactory{};
    auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
    OrcaDebug::GraphicsLog("IDXGIFactory4��������", hr);

    // �X���b�v�`�F�[���̏�����
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

    // �X���b�v�`�F�[�����쐬
    IDXGISwapChain* pSwapChain{};
    hr = pFactory->CreateSwapChain(mpCommandQueue.Get(), &desc, &pSwapChain);
    OrcaDebug::GraphicsLog("�X���b�v�`�F�[�����쐬", hr);

    // �X���b�v�`�F�[�����擾
    hr = pSwapChain->QueryInterface(IID_PPV_ARGS(mpSwapChain.ReleaseAndGetAddressOf()));
    OrcaDebug::GraphicsLog("�X���b�v�`�F�[�����擾", hr);

    // �o�b�N�o�b�t�@�̔ԍ����擾
    mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();

    // �v�f�����
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
        OrcaDebug::GraphicsLog("�R�}���h�A���P�[�^�[��������", hr);
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

    OrcaDebug::GraphicsLog("�R�}���h���X�g���쐬", hr);
}

void OrcaGraphics::Graphics::CreateRenderTargetView()
{
    // �f�B�X�N���v�^�q�[�v�̐ݒ�
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.NumDescriptors = Orca::FrameCount;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask = 0;

    // �f�B�X�N���v�^�q�[�v�̍쐬
    auto hr = mpDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(mpHeapRTV.ReleaseAndGetAddressOf()));
    OrcaDebug::GraphicsLog("�f�B�X�N���v�^�q�[�v�̍쐬", hr);


    auto handle = mpHeapRTV->GetCPUDescriptorHandleForHeapStart();
    // ���̃T�C�Y��GPU�Ɉˑ����Ă���̂ŌŒ�l�͎g���Ă����Ȃ�
    const auto incrementSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // �����_�[�^�[�Q�b�g�r���[�̐ݒ�
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    rtvDesc.Texture2D.PlaneSlice = 0;

    for (int i = 0; i < Orca::FrameCount; ++i)
    {
        hr = mpSwapChain->GetBuffer(i, IID_PPV_ARGS(mpColorBuffer[i].ReleaseAndGetAddressOf()));
        OrcaDebug::GraphicsLog("�o�b�t�@���擾", hr);

        mpDevice->CreateRenderTargetView(mpColorBuffer[i].Get(), &rtvDesc, handle);
        mHandleRTV[i] = handle;
        handle.ptr += incrementSize;
    }


}

void OrcaGraphics::Graphics::CreateFence()
{
    // �t�F���X�I�u�W�F�N�g���쐬
    const auto hr = mpDevice->CreateFence(
        mFenceCounter[mFrameIndex],
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(&mpFence)
    );

    OrcaDebug::GraphicsLog("�t�F���X�I�u�W�F�N�g�𐶐�", hr);

    // �t�F���X�C�x���g���쐬
    mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    Orca_NullExeption(mFenceEvent)

}

void OrcaGraphics::Graphics::Present(uint32_t Interval_)
{
    // ��ʂɕ\��
    mpSwapChain->Present(Interval_, 0);

    // �V�O�i������
    const auto currentValue = mFenceCounter[mFrameIndex];
    mpCommandQueue->Signal(mpFence.Get(), currentValue);

    // �o�b�N�o�b�t�@�ԍ����X�V
    mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();

    // ���̃t���[���̕`�揀�����܂��ł���Αҋ@����
    if(mpFence->GetCompletedValue()<mFenceCounter[mFrameIndex])
    {
        mpFence->SetEventOnCompletion(mFenceCounter[mFrameIndex], mFenceEvent);
        WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);
    }

    // ���̃t���[���̃J�E���^�[�𑝂₷
    mFenceCounter[mFrameIndex] = currentValue + 1;
}

void OrcaGraphics::Graphics::WaitGpu()
{
    Orca_NullExeption(mpCommandQueue)
    Orca_NullExeption(mpFence)
    Orca_NullExeption(mFenceEvent)

    // �V�O�i������
    mpCommandQueue->Signal(mpFence.Get(), mFenceCounter[mFrameIndex]);
    // �������ɃC�x���g��ݒ肷��
    mpFence->SetEventOnCompletion(mFenceCounter[mFrameIndex], mFenceEvent);
    // �ҋ@����
    WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);

    // �J�E���^�[�𑝂₷
    mFenceCounter[mFrameIndex]++;

}

void OrcaGraphics::Graphics::AddDebugFlag() const
{
    // �f�o�b�O���[�h�݂̂��̊֐���L���ɂ���
#if defined(DEBUG)||defined(_DEBUG)
    Microsoft::WRL::ComPtr<ID3D12Debug> pDebug{};
    const auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(pDebug.GetAddressOf()));

    // �f�o�b�O���C���[��L����
    if(SUCCEEDED(hr))
    {
        pDebug->EnableDebugLayer();
    }
#endif
}
