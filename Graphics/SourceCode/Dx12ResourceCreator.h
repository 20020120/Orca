//#pragma once
//#include"Dx12ResourceInfo.h"
//
//#include<unordered_map>
//#include<memory>
//// ------------------------------------ �O���錾 -----------------------------------
//namespace Graphics
//{
//    namespace Resource
//    {
//        class ConstantBuffer;
//        class Texture;
//        class Dx12Resource;
//    }
//}
//
//// -------------------------------- ���\�[�X�𐶐�����N���X -------------------------------
//namespace Graphics
//{
//    namespace Resource
//    {
//        class Dx12ResourceCreator
//        {
//        public:
//            static void CreateResource(std::unordered_map<std::string, std::shared_ptr<Dx12Resource>>& Holder_,
//                const Dx12ResourceInfo& Info_);
//
//
//        private:
//            // ------------------------ ���\�[�X���R���e�i�ɒǉ�����֐� -----------------------
//            static void Add(std::unordered_map<std::string, std::shared_ptr<Dx12Resource>>& Holder_,
//                std::shared_ptr<Dx12Resource> pResource_);
//
//            // ------------------------- �e���\�[�X�𐶐�����֐� ------------------------
//            static std::shared_ptr<ConstantBuffer> CreateConstantBuffer(const Dx12ResourceInfo& Info_);
//            static std::shared_ptr<Texture> CreateTexture(const Dx12ResourceInfo& Info_);
//        };
//    }
//}
