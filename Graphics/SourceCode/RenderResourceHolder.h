#pragma once
#include<vector>
#include<memory>
namespace OrcaGraphics
{
    class RenderResource;

    // �`��p���\�[�X�̕ێ��N���X
    class RenderResourceHolder
    {
        // ---------------------------- �e���\�[�X��ǉ�����֐� ---------------------------
        void AddConstantBuffer(void* MappedPtr_);
        void AddTexture(const wchar_t* Path_);

    private:
        std::vector<std::unique_ptr<RenderResource>> mResources{};
    };
}
