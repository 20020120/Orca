#include"pch.h"
#include"Graphics.h"
#include"GraphicsLogger.h"

OrcaGraphics::Graphics::Graphics()= default;
OrcaGraphics::Graphics::~Graphics() = default;


void OrcaGraphics::Graphics::Initialize()
{
    // ----------------------------- DirectX12の初期化 -----------------------------
    CreateDevice();

}

void OrcaGraphics::Graphics::CreateDevice()
{
    // デバイスの初期化
    const auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(mpDevice.ReleaseAndGetAddressOf()));

    OrcaDebug::GraphicsLog(hr);
}
