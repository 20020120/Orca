#include"pch.h"
//#include"Dx12ResourceCreator.h"
//
//#include"ConstantBuffer.h"
//#include"Texture.h"
//
//void Graphics::Resource::Dx12ResourceCreator::CreateResource(
//    std::unordered_map<std::string, std::shared_ptr<Dx12Resource>>& Holder_,
//    const Dx12ResourceInfo& Info_)
//{
//    // ----------------------------- Info���烊�\�[�X�𐶐� -----------------------------
//    switch (Info_.mType)
//    {
//    case ResourceType::Unknown: 
//        break;
//    case ResourceType::ConstantBuffer:
//        Add(Holder_, CreateConstantBuffer(Info_));
//        return;
//    case ResourceType::Sampler: break;
//    case ResourceType::Texture:
//        Add(Holder_, CreateTexture(Info_));
//        return;
//    case ResourceType::UAV: break;
//    default: ;
//    }
//}
//
//
//void Graphics::Resource::Dx12ResourceCreator::Add(std::unordered_map<std::string, std::shared_ptr<Dx12Resource>>& Holder_,
//    std::shared_ptr<Dx12Resource> pResource_)
//{
//    // ------------------------------- ���\�[�X��ǉ����� -------------------------------
//    Holder_.try_emplace(pResource_->GetName(), pResource_);
//}
//
//std::shared_ptr<Graphics::Resource::ConstantBuffer> Graphics::Resource::Dx12ResourceCreator::CreateConstantBuffer(
//    const Dx12ResourceInfo& Info_)
//{
//    // ------------------------------- �萔�o�b�t�@���쐬 -------------------------------
//    return std::make_shared<ConstantBuffer>(Info_.mName, Info_.mBufferSize, Info_.mRootIndex);
//}
//
//std::shared_ptr<Graphics::Resource::Texture> Graphics::Resource::Dx12ResourceCreator::CreateTexture(
//    const Dx12ResourceInfo& Info_)
//{
//    return std::make_shared<Texture>(Info_.mName, Info_.mRootIndex);
//}
//
////std::shared_ptr<Graphics::Resource::Texture> Graphics::Resource::Dx12ResourceCreator::CreateTexture(
////    const Dx12ResourceInfo& Info_)
////{
////    return std::make_shared<Texture>();
////}
