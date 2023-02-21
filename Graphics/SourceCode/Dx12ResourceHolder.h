//#pragma once
//#include"ShaderType.h"
//#include"ConstantBuffer.h"
//#include"Texture.h"
//#include"Graphics.h"
//#include"MacroMemory.h"
//
//#include<string>
//#include<map>
//#include <unordered_map>
//#include<memory>
//#include<ResourceUploadBatch.h>
//struct ID3D12GraphicsCommandList;
//namespace OrcaGraphics
//{
//    namespace Resource
//    {
//        class Dx12Resource;
//    }
//    // �`��p���\�[�X�̕ێ��N���X
//    class Dx12ResourceHolder
//    {
//        // �I�u�W�F�N�g�̖��O/�ϐ���/���\�[�X
//        using HolderType = std::map<uint64_t, std::unordered_map<std::string, std::shared_ptr<Resource::Dx12Resource>>>;
//    public:
//        static uint64_t Add(const Shader::ShaderType& ShaderType_);  // �V�F�[�_�[�̃��\�[�X����V�K�ɒǉ�����
//        static void StackGraphicsCmd(ID3D12GraphicsCommandList* pCmdList_, uint64_t Handle_);
//        static void Finalize(); // �I������
//        
//        template<class T>
//        static void CbMapping(uint64_t Handle_, std::string VariableName_, T** MappedPtr_);   // �萔�o�b�t�@��CPU�̃C���^�[�t�F�[�X���}�b�s���O����
//        /**
//         * \brief 
//         * \tparam Pair std::tuple<std::string(�ϐ���),std::wstrnig(�e�N�X�`���̃p�X)>
//         * \param Handle �I�u�W�F�N�g���Ƃ̃n���h��
//         * \param Pairs_ Pair�������ς�
//         */
//        template<class ...Pair>
//        static void TexLoad(uint64_t Handle, Pair&& ...Pairs_);
//
//
//    private:
//        inline static HolderType mHolder{};
//        inline static uint64_t mHandler{};
//        
//        // --------------------- �R���X�g���N�^���B���ĐV�K�ɐ����ł��Ȃ��悤�ɂ��� ---------------------
//    private:
//        Dx12ResourceHolder() = default;
//    };
//
//    template <class T>
//    void Dx12ResourceHolder::CbMapping(const uint64_t Handle_, std::string VariableName_, T** MappedPtr_)
//    {
//        const auto& resource = mHolder.at(Handle_).at(VariableName_);
//        if(const auto& cb = std::dynamic_pointer_cast<Resource::ConstantBuffer>(resource))
//        {
//            cb->Mapping(reinterpret_cast<void**>(MappedPtr_));
//        }
//    }
//
//    template <class ... Pair>
//    void Dx12ResourceHolder::TexLoad(uint64_t Handle_, Pair&&... Pairs_)
//    {
//        if(!mHolder.contains(Handle_))
//            return;
//
//        // �e�N�X�`�����쐬
//        DirectX::ResourceUploadBatch batch(Graphics::GetDevice().Get());
//        batch.Begin();
//      
//        const auto& map = mHolder.at(Handle_);
//        for (const auto& [variableName,texPath] : std::initializer_list<std::tuple<std::string, std::wstring>>{std::forward<Pair>(Pairs_)...})
//        {
//            if (!map.contains(variableName))
//                continue;
//            const auto& res = map.at(variableName);
//            if (const auto tex = ORCA_DYNAMIC_CAST(OrcaGraphics::Resource::Texture,res))
//            {
//                tex->Load(texPath.c_str(), batch);
//            }
//        }
//        // �R�}���h�����s
//        const auto future = batch.End(Graphics::GetCommandQueue().Get());
//        future.wait();
//    }
//}
