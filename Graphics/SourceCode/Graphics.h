#pragma once
#include"GraphicsMacro.h"
#include"MacroMinmam.h"
#include "GraphicsMinimum.h"
#include<memory>
// ------------------------------------ 前方宣言 -----------------------------------
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
    // DirectX12の描画クラス
    class Graphics
    {
    public:
        // -------------------------------- 公開関数 -------------------------------
        static OrcaNodiscard OrcaComPtr(ID3D12Device) GetDevice();
        static OrcaNodiscard OrcaComPtr(ID3D12GraphicsCommandList) GetCmdList();
        static OrcaNodiscard DescriptorPool* GetDescriptorPool(POOL_TYPE Type_);
        static OrcaNodiscard OrcaComPtr(ID3D12CommandQueue) GetCommandQueue();

    protected:
        // コンストラクタを隠しておく
        Graphics() = default;

        static std::unique_ptr<GraphicsImpl> mpImpl;
    };
}
