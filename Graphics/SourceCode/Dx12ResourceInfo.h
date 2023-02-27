#pragma once
#include<string>
// ---------------------------- �V�F�[�_�[�̃��\�[�X�𐶐����邽�߂̏�� ---------------------------
namespace Graphics
{
    namespace Resource
    {
        enum class ResourceType
        {
            Unknown,
            ConstantBuffer,
            Sampler,
            Texture,
            UAV,
        };

        struct Dx12ResourceInfo
        {
            ResourceType mType{};   // ���\�[�X�̎��
            std::string mName{};    // ���\�[�X�̖��O�iHLSL�ł̕ϐ����j
            size_t mBufferSize{};   // �o�b�t�@�̑傫��
            uint32_t mRootIndex{};  // �o�C���h���̃C���f�b�N�X
        };
    }
}
