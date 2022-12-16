#include"pch.h"
#include"Graphics.h"
#include"GraphicsLogger.h"
#include<dxgi1_4.h>
#include<d3d12.h>
#include"ScreenConstants.h"
OrcaGraphics::Graphics::Graphics()= default;
OrcaGraphics::Graphics::~Graphics()
{
    Finalize();
}


void OrcaGraphics::Graphics::Initialize(HWND hWnd_)
{
    // ----------------------------- DirectX12�̏����� -----------------------------
    CreateDevice();
    CreateCommandQueue();
    CreateSwapChain(hWnd_);

}

void OrcaGraphics::Graphics::Finalize()
{
    // ---------------------------------- �I������ ---------------------------------

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
