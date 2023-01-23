#pragma once
#include "Dx12ResourceInfo.h"
#include"ShaderType.h"
#include<map>
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
        using HolderType = std::multimap<std::string, std::map<std::string, std::shared_ptr<Resource::Dx12Resource>>>;
    public:
        template<class T>
        static void CbMapping(std::string ObjectName_, std::string VariableName_, T* MappedPtr_);   // �萔�o�b�t�@��CPU�̃C���^�[�t�F�[�X���}�b�s���O����

        static void Add(Shader::ShaderType, std::string ObjectName_);  // �V�F�[�_�[�̃��\�[�X����V�K�ɒǉ�����


    private:
        inline static HolderType mHolder{};


        // --------------------- �R���X�g���N�^���B���ĐV�K�ɐ����ł��Ȃ��悤�ɂ��� ---------------------
    private:
        Dx12ResourceHolder() = default;

    };
}
