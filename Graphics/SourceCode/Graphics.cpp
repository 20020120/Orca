#include"pch.h"
#include"Graphics.h"
#include"GraphicsLogger.h"
#include<dxgi1_4.h>
#include<d3d12.h>
#include<d3dcompiler.h>
#include"ResourceUploadBatch.h"
#include"DDSTextureLoader.h"


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
    CreateVertexBuffer();
    CreateConstantBuffer();
    CreateRootSignature();
    CreateDepthBuffer();
    CreateGPS();
    CreateViewport();
    CreateScissor();
    CreateIndexBuffer();
    CreateTexture();
}

void OrcaGraphics::Graphics::Finalize()
{
    // ---------------------------------- �I������ ---------------------------------
    WaitGpu();
}

void OrcaGraphics::Graphics::OnTerm()
{
    for (int i = 0; i < Orca::FrameCount; ++i)
    {
        if(mpConstantBuffer)
        {
            mpConstantBuffer[i]->Unmap(0, nullptr);
            memset(&mCbV[i], 0, sizeof(mCbV[i]));
        }
        mpConstantBuffer->Reset();
    }
    mpVertexBuffer.Reset();
    mpPSO.Reset();
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
    mpCommandList->OMSetRenderTargets(1, &mHandleRTV[mFrameIndex], FALSE, &mHandleDsV);

    // �N���A�J���[�̐ݒ�
    constexpr  float clearColor[] = { 0.3f,0.3f,0.3f,1.0f };
    // �����_�[�^�[�Q�b�g���N���A
    mpCommandList->ClearRenderTargetView(mHandleRTV[mFrameIndex], clearColor, 0, nullptr);
    mpCommandList->ClearDepthStencilView(mHandleDsV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // �X�V����
    {
        mRotateAngle -= 0.025f;
        mCbV[mFrameIndex * 2 + 0].mpBuffer->World =
            DirectX::XMMatrixRotationY(mRotateAngle + DirectX::XMConvertToRadians(45.0f));
        mCbV[mFrameIndex * 2 + 1].mpBuffer->World = 
            DirectX::XMMatrixRotationY(mRotateAngle)*DirectX::XMMatrixScaling(2.0f, 0.5f, 1.0f);

    }

    // �`�揈��
    {
        mpCommandList->SetGraphicsRootSignature(mpRootSignature.Get());
        mpCommandList->SetDescriptorHeaps(1, mpHeapCbV_SRV_UAV.GetAddressOf());
        mpCommandList->SetPipelineState(mpPSO.Get());
        mpCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        mpCommandList->IASetVertexBuffers(0, 1, &mVbView);
        mpCommandList->IASetIndexBuffer(&mIbView);
        mpCommandList->RSSetViewports(1, &mViewPort);
        mpCommandList->RSSetScissorRects(1, &mScissor);

        mpCommandList->SetGraphicsRootConstantBufferView(0, mCbV[mFrameIndex * 2 + 0].mDesc.BufferLocation);
        mpCommandList->SetGraphicsRootDescriptorTable(1, mTexture.mHandleGPU);
        mpCommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
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
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

void OrcaGraphics::Graphics::CreateIndexBuffer()
{
    // �C���f�b�N�X�f�[�^��p��
    const uint32_t indices[] = { 0,1,2,0,2,3 };

    // �q�[�v�v���p�e�B��ݒ�
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_UPLOAD;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // ���\�[�X�̐ݒ�
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeof(indices);
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // ���\�[�X�𐶐�
    auto hr = mpDevice->CreateCommittedResource(
        &prp,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpIndexBuffer.ReleaseAndGetAddressOf())
    );
    OrcaDebug::GraphicsLog("�C���f�b�N�X�o�b�t�@���쐬", hr);

    // �}�b�s���O
    void* ptr = nullptr;
    hr = mpIndexBuffer->Map(0, nullptr, &ptr);
    OrcaDebug::GraphicsLog("�}�b�s���O", hr);
    memcpy(ptr, indices, sizeof(indices));
    mpIndexBuffer->Unmap(0, nullptr);

    // �C���f�b�N�X�o�b�t�@�r���[���쐬
    mIbView.BufferLocation = mpIndexBuffer->GetGPUVirtualAddress();
    mIbView.Format = DXGI_FORMAT_R32_UINT;
    mIbView.SizeInBytes = sizeof(indices);
}

void OrcaGraphics::Graphics::CreateDepthBuffer()
{
    // �q�[�v�v���p�e�B��ݒ�
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_DEFAULT;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // ���\�[�X�̐ݒ�
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
    OrcaDebug::GraphicsLog("�f�v�X�o�b�t�@���쐬", hr);

    // �f�B�X�N���v�^�q�[�v�̐ݒ�
    D3D12_DESCRIPTOR_HEAP_DESC descDH{};
    descDH.NumDescriptors = 1;
    descDH.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    descDH.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    descDH.NodeMask = 0;
    hr = mpDevice->CreateDescriptorHeap(
        &descDH,
        IID_PPV_ARGS(mpHeapDSV.ReleaseAndGetAddressOf())
    );
    OrcaDebug::GraphicsLog("�f�B�X�N���v�^�q�[�v���쐬", hr);

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

void OrcaGraphics::Graphics::CreateVertexBuffer()
{
    constexpr Vertex vertices[] = {
        {DirectX::XMFLOAT3(-1.0f,1.0f,0.0f),DirectX::XMFLOAT2(0.0f,0.0f)},
        {DirectX::XMFLOAT3(1.0f,1.0f,0.0f),DirectX::XMFLOAT2(1.0f,0.0f)},
        {DirectX::XMFLOAT3(1.0f,-1.0f,0.0f),DirectX::XMFLOAT2(1.0f,1.0f)},
        {DirectX::XMFLOAT3(-1.0f,-1.0f,0.0f),DirectX::XMFLOAT2(0.0f,1.0f)}
    };

    // �q�[�v�v���p�e�B�̐ݒ�
    D3D12_HEAP_PROPERTIES prp{};
    prp.Type = D3D12_HEAP_TYPE_UPLOAD;
    prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prp.CreationNodeMask = 1;
    prp.VisibleNodeMask = 1;

    // ���\�[�X�̐ݒ�
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = sizeof(vertices);
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    auto hr = mpDevice->CreateCommittedResource(
        &prp,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(mpVertexBuffer.GetAddressOf())
    );

    OrcaDebug::GraphicsLog("���_�o�b�t�@���쐬", hr);

    // �}�b�s���O����
    void* ptr{};
    hr = mpVertexBuffer->Map(0, nullptr, &ptr);
    OrcaDebug::GraphicsLog("�}�b�s���O", hr);

    // ���_�f�[�^���}�b�s���O��Ɏw��
    memcpy(ptr, vertices, sizeof(vertices));

    // �}�b�s���O����
    mpVertexBuffer->Unmap(0, nullptr);

    // �r���[�̐ݒ�
    mVbView.BufferLocation = mpVertexBuffer->GetGPUVirtualAddress();
    mVbView.SizeInBytes = static_cast<UINT>(sizeof(vertices));
    mVbView.StrideInBytes = sizeof(Vertex);
}

void OrcaGraphics::Graphics::CreateConstantBuffer()
{
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 2 * Orca::FrameCount;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.NodeMask = 0;

        const auto hr = mpDevice->CreateDescriptorHeap(
            &desc,
            IID_PPV_ARGS(mpHeapCbV_SRV_UAV.ReleaseAndGetAddressOf()));

        OrcaDebug::GraphicsLog("�f�B�X�N���v�^�q�[�v���쐬", hr);
    }

    {
        // �q�[�v�v���p�e�B�̐ݒ�
        D3D12_HEAP_PROPERTIES prp{};
        prp.Type = D3D12_HEAP_TYPE_UPLOAD;
        prp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prp.CreationNodeMask = 1;
        prp.VisibleNodeMask = 1;

        // ���\�[�X�̐ݒ�
        D3D12_RESOURCE_DESC desc{};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = sizeof(CB_Simple);
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;
        const auto incrementSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);;

        for (int i = 0; i < Orca::FrameCount*2; ++i)
        {
            // ���\�[�X����
            auto hr = mpDevice->CreateCommittedResource(
                &prp,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(mpConstantBuffer[i].ReleaseAndGetAddressOf())
            );
            OrcaDebug::GraphicsLog("�萔�o�b�t�@���쐬", hr);

            const auto address = mpConstantBuffer[i]->GetGPUVirtualAddress();
            auto handleCPU = mpHeapCbV_SRV_UAV->GetCPUDescriptorHandleForHeapStart();
            auto handleGPU = mpHeapCbV_SRV_UAV->GetGPUDescriptorHandleForHeapStart();
            handleCPU.ptr += incrementSize * i;
            handleGPU.ptr += incrementSize * i;

            // �萔�o�b�t�@�����[�̐ݒ�
            mCbV[i].mHandleCPU = handleCPU;
            mCbV[i].mHandleGPU = handleGPU;
            mCbV[i].mDesc.BufferLocation = address;
            mCbV[i].mDesc.SizeInBytes = sizeof(CB_Simple);

            // �萔�o�b�t�@�r���[���쐬
            mpDevice->CreateConstantBufferView(&mCbV[i].mDesc, handleCPU);

            // �}�b�s���O
            hr = mpConstantBuffer[i]->Map(0, nullptr,
                reinterpret_cast<void**>(&mCbV[i].mpBuffer));

            OrcaDebug::GraphicsLog("�萔�o�b�t�@���}�b�s���O", hr);

            const auto eyePos = DirectX::XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
            const auto targetPos = DirectX::XMVectorZero();
            const auto upward = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            constexpr auto fovY = DirectX::XMConvertToRadians(37.5f);
            constexpr auto aspect = Orca::ScreenWidth / Orca::ScreenHeight;

            // �ϊ��s��̐ݒ�
            mCbV[i].mpBuffer->World = DirectX::XMMatrixIdentity();
            mCbV[i].mpBuffer->ViewMat = DirectX::XMMatrixLookAtLH(eyePos, targetPos, upward);
            mCbV[i].mpBuffer->ProjMat = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, 1.0f, 1000.0f);
        }
    }
}

void OrcaGraphics::Graphics::CreateRootSignature()
{
    // ���[�g�V�O�l�`���̍쐬
    {
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        // ���[�g�p�����[�^�̐ݒ�
        D3D12_ROOT_PARAMETER param[2]{};
        param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param[0].Descriptor.ShaderRegister = 0;
        param[0].Descriptor.RegisterSpace = 0;
        param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

        D3D12_DESCRIPTOR_RANGE range{};
        range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        range.NumDescriptors = 1;
        range.BaseShaderRegister = 0;
        range.RegisterSpace = 0;
        range.OffsetInDescriptorsFromTableStart = 0;

        param[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param[1].DescriptorTable.NumDescriptorRanges = 1;
        param[1].DescriptorTable.pDescriptorRanges = &range;
        param[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        // �X�^�e�B�b�N�T���v���[�̐ݒ�
        D3D12_STATIC_SAMPLER_DESC descSSD{};
        descSSD.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
        descSSD.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        descSSD.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        descSSD.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        descSSD.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
        descSSD.MaxAnisotropy = 1;
        descSSD.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        descSSD.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        descSSD.MinLOD = -D3D12_FLOAT32_MAX;
        descSSD.MaxLOD = +D3D12_FLOAT32_MAX;
        descSSD.ShaderRegister = 0;
        descSSD.RegisterSpace = 0;
        descSSD.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


        // ���[�g�V�O�l�`���̐ݒ�
        D3D12_ROOT_SIGNATURE_DESC desc{};
        desc.NumParameters = 2;
        desc.NumStaticSamplers = 1;
        desc.pParameters = param;
        desc.pStaticSamplers = &descSSD;
        desc.Flags = flag;

        Microsoft::WRL::ComPtr<ID3DBlob> pBlob{};
        Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob{};
        // �V���A���C�Y
        auto hr = D3D12SerializeRootSignature(&desc,
            D3D_ROOT_SIGNATURE_VERSION_1_0,
            pBlob.GetAddressOf(),
            pErrorBlob.GetAddressOf()
        );
        OrcaDebug::GraphicsLog("���[�g�V�O�l�`�����V���A���C�Y", hr);

        // ���[�g�V�O�l�`���𐶐�
        hr = mpDevice->CreateRootSignature(
            0,
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            IID_PPV_ARGS(mpRootSignature.GetAddressOf())
        );
        OrcaDebug::GraphicsLog("���[�g�V�O�l�`���𐶐�", hr);
    }
}

void OrcaGraphics::Graphics::CreateGPS()
{
    // �C���v�b�g���C�A�E�g���쐬
    D3D12_INPUT_ELEMENT_DESC elements[2]{};
    elements[0].SemanticName = "POSITION";
    elements[0].SemanticIndex = 0;
    elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    elements[0].InputSlot = 0;
    elements[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    elements[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    elements[0].InstanceDataStepRate = 0;

    elements[1].SemanticName = "TEXCOORD";
    elements[1].SemanticIndex = 0;
    elements[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    elements[1].InputSlot = 0;
    elements[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    elements[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
    elements[1].InstanceDataStepRate = 0;


    // ���X�^���C�U�[�X�e�[�g�̐ݒ�
    D3D12_RASTERIZER_DESC descRS{};
    descRS.FillMode = D3D12_FILL_MODE_SOLID;
    descRS.CullMode = D3D12_CULL_MODE_NONE;
    descRS.FrontCounterClockwise = FALSE;
    descRS.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    descRS.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    descRS.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    descRS.DepthClipEnable = FALSE;
    descRS.MultisampleEnable = FALSE;
    descRS.AntialiasedLineEnable = FALSE;
    descRS.ForcedSampleCount = 0;
    descRS.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�.
    D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
        FALSE, FALSE,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL
    };


    // �u�����h�X�e�[�g�̐ݒ�.
    D3D12_BLEND_DESC descBS;
    descBS.AlphaToCoverageEnable = FALSE;
    descBS.IndependentBlendEnable = FALSE;
    for (auto& i : descBS.RenderTarget)
    {
        i = descRTBS;
    }
    // �f�v�X�X�e���V���X�e�[�g��ݒ�
    D3D12_DEPTH_STENCIL_DESC descDSS{};
    descDSS.DepthEnable = TRUE;
    descDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    descDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    descDSS.StencilEnable = FALSE;



    // ���_�V�F�[�_�[�ǂݍ���
    Microsoft::WRL::ComPtr<ID3DBlob> pVsBlob{};
    auto hr = D3DReadFileToBlob(L"../Resource/Shader/SimpleTexVs.cso", pVsBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("���_�V�F�[�_�[��ǂݍ���", hr);

    // �s�N�Z���V�F�[�_�[�ǂݍ���
    Microsoft::WRL::ComPtr<ID3DBlob> pPsBlob{};
    hr = D3DReadFileToBlob(L"../Resource/Shader/SimpleTexPs.cso", pPsBlob.ReleaseAndGetAddressOf());
    OrcaDebug::GraphicsLog("�s�N�Z���V�F�[�_�[��ǂݍ���", hr);


    // �O���t�B�b�N�X�p�C�v���C���I�u�W�F�N�g�𐶐�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descGps{};
    descGps.InputLayout = { elements, _countof(elements) };
    descGps.pRootSignature = mpRootSignature.Get();
    descGps.VS = { pVsBlob->GetBufferPointer(),pVsBlob->GetBufferSize() };
    descGps.PS = { pPsBlob->GetBufferPointer(),pPsBlob->GetBufferSize() };
    descGps.RasterizerState = descRS;
    descGps.BlendState = descBS;
    descGps.DepthStencilState = descDSS;
    descGps.SampleMask = UINT_MAX;
    descGps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    descGps.NumRenderTargets = 1;
    descGps.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    descGps.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    descGps.SampleDesc = { 1,0 };

    // �p�C�v���C���X�e�[�g��ݒ�
    hr = mpDevice->CreateGraphicsPipelineState(
        &descGps, IID_PPV_ARGS(mpPSO.GetAddressOf()));

    OrcaDebug::GraphicsLog("�p�C�v���C���X�e�[�g���쐬", hr);
}

void OrcaGraphics::Graphics::CreateViewport()
{
    // �r���[�|�[�g���쐬����
    mViewPort.TopLeftX = 0.0f;
    mViewPort.TopLeftY = 0.0f;
    mViewPort.Width = Orca::ScreenWidth;
    mViewPort.Height = Orca::ScreenHeight;
    mViewPort.MinDepth = 0.0f;
    mViewPort.MaxDepth = 1.0f;
}

void OrcaGraphics::Graphics::CreateScissor()
{
    // �V�U�[��`���쐬
    mScissor.left = 0;
    mScissor.right = Orca::ScreenWidth;
    mScissor.top = 0;
    mScissor.bottom = Orca::ScreenHeight;
}

void OrcaGraphics::Graphics::CreateTexture()
{
    DirectX::ResourceUploadBatch batch(mpDevice.Get());
    batch.Begin();
    // ���\�[�X�𐶐�

    const auto hr = DirectX::CreateDDSTextureFromFile(
        mpDevice.Get(),
        batch,
        L"../Resource/Sprite/Test.dds",
        mTexture.mpResource.ReleaseAndGetAddressOf()
    );
    OrcaDebug::GraphicsLog("�e�N�X�`���𐶐�", hr);

    // �R�}���h�����s
    const auto future = batch.End(mpCommandQueue.Get());
    future.wait();

    const auto incrementSize = mpDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // �b�o�t�n���h�����擾
    auto handleCPU = mpHeapCbV_SRV_UAV->GetCPUDescriptorHandleForHeapStart();
    auto handleGPU = mpHeapCbV_SRV_UAV->GetGPUDescriptorHandleForHeapStart();
    handleCPU.ptr += incrementSize * 2;
    handleGPU.ptr += incrementSize * 2;
    mTexture.mHandleCPU = handleCPU;
    mTexture.mHandleGPU = handleGPU;

    auto textureDesc = mTexture.mpResource->GetDesc();
    // �V�F�[�_�[���\�[�X�r���[��ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC descSRV{};
    descSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    descSRV.Format = textureDesc.Format;
    descSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    descSRV.Texture2D.MostDetailedMip = 0;
    descSRV.Texture2D.MipLevels = textureDesc.MipLevels;
    descSRV.Texture2D.PlaneSlice = 0;
    descSRV.Texture2D.ResourceMinLODClamp = 0.0f;

    // �V�F�[�_�[���\�[�X�r���[���쐬
    mpDevice->CreateShaderResourceView(mTexture.mpResource.Get(), &descSRV, handleCPU);


}
