#pragma once
#include"ShaderType.h"
#include<string>
#include<map>
#include <unordered_map>
#include<memory>
namespace OrcaGraphics
{
    namespace Resource
    {
        class Dx12Resource;
    }
    // �`��p���\�[�X�̕ێ��N���X
    class Dx12ResourceHolder
    {
        // �I�u�W�F�N�g�̖��O/�ϐ���/���\�[�X
        using HolderType = std::map<std::string, std::unordered_map<std::string, std::shared_ptr<Resource::Dx12Resource>>>;
    public:
        template<class T>
        static void CbMapping(std::string ObjectName_, std::string VariableName_, T* MappedPtr_);   // �萔�o�b�t�@��CPU�̃C���^�[�t�F�[�X���}�b�s���O����

        static void Add(const Shader::ShaderType& ShaderType_,
            const std::string& ObjectName_);  // �V�F�[�_�[�̃��\�[�X����V�K�ɒǉ�����
        
    private:
        inline static HolderType mHolder{};
        
        // --------------------- �R���X�g���N�^���B���ĐV�K�ɐ����ł��Ȃ��悤�ɂ��� ---------------------
    private:
        Dx12ResourceHolder() = default;

    };
}
