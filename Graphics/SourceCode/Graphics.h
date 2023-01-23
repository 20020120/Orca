#pragma once
#include"GraphicsMacro.h"
#include"MacroMinmam.h"
#include "GraphicsMinimum.h"
#include<memory>
// ------------------------------------ �O���錾 -----------------------------------
namespace OrcaGraphics
{
    class GraphicsImpl;
    class DescriptorPool;
}

struct ID3D12Device;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;

namespace OrcaGraphics
{
    // DirectX12�̕`��N���X
    class Graphics
    {
    public:
        // -------------------------------- ���J�֐� -------------------------------
        static OrcaNodiscard OrcaComPtr(ID3D12Device) GetDevice();
        static OrcaNodiscard OrcaComPtr(ID3D12GraphicsCommandList) GetCmdList();
        static OrcaNodiscard DescriptorPool* GetDescriptorPool(POOL_TYPE Type_);
        static OrcaNodiscard OrcaComPtr(ID3D12CommandQueue) GetCommandQueue();

    protected:
        // �R���X�g���N�^���B���Ă���
        Graphics() = default;

        static std::unique_ptr<GraphicsImpl> mpImpl;
    };
}
