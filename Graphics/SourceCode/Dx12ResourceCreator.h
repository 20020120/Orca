//#pragma once
//#include"Dx12ResourceInfo.h"
//
//#include<unordered_map>
//#include<memory>
//// ------------------------------------ 前方宣言 -----------------------------------
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
//// -------------------------------- リソースを生成するクラス -------------------------------
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
//            // ------------------------ リソースをコンテナに追加する関数 -----------------------
//            static void Add(std::unordered_map<std::string, std::shared_ptr<Dx12Resource>>& Holder_,
//                std::shared_ptr<Dx12Resource> pResource_);
//
//            // ------------------------- 各リソースを生成する関数 ------------------------
//            static std::shared_ptr<ConstantBuffer> CreateConstantBuffer(const Dx12ResourceInfo& Info_);
//            static std::shared_ptr<Texture> CreateTexture(const Dx12ResourceInfo& Info_);
//        };
//    }
//}
