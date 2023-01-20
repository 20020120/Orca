#pragma once
#include<vector>
#include<memory>
namespace OrcaGraphics
{
    class Dx12Resource;

    // 描画用リソースの保持クラス
    class RenderResourceHolder
    {
        // ---------------------------- 各リソースを追加する関数 ---------------------------
        void AddConstantBuffer(void* MappedPtr_);
        void AddTexture(const wchar_t* Path_);

    private:
        std::vector<std::unique_ptr<Dx12Resource>> mResources{};
    };
}
