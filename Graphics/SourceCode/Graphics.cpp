#include"pch.h"
#include"Graphics.h"
#include"GraphicsLogger.h"
#include<dxgi1_4.h>
OrcaGraphics::Graphics::Graphics()= default;
OrcaGraphics::Graphics::~Graphics() = default;


void OrcaGraphics::Graphics::Initialize()
{
    // ----------------------------- DirectX12�̏����� -----------------------------
    CreateDevice();
    CreateCommandQueue();
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

void OrcaGraphics::Graphics::CreateSwapChain()
{
    IDXGIFactory4* pFactory{};
    const auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
    OrcaDebug::GraphicsLog("IDXGIFactory4��������", hr);

    // �X���b�v�`�F�[���̏�����



}
