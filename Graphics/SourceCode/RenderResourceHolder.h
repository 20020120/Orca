#pragma once
#include<vector>
#include<memory>
namespace OrcaGraphics
{
    class Dx12Resource;

    // �`��p���\�[�X�̕ێ��N���X
    class RenderResourceHolder
    {
        // ---------------------------- �e���\�[�X��ǉ�����֐� ---------------------------
        void AddConstantBuffer(void* MappedPtr_);
        void AddTexture(const wchar_t* Path_);

    private:
        std::vector<std::unique_ptr<Dx12Resource>> mResources{};
    };
}
