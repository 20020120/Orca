#include"pch.h"
#include"Dx12ResourceCreator.h"

#include"ConstantBuffer.h"

void OrcaGraphics::Resource::Dx12ResourceCreator::CreateResource(
    std::unordered_map<std::string, std::shared_ptr<Resource::Dx12Resource>>& Holder_,
    const OrcaGraphics::Resource::Dx12ResourceInfo& Info_)
{
    // ----------------------------- Infoからリソースを生成 -----------------------------
    switch (Info_.mType)
    {
    case ResourceType::Unknown: 
        break;
    case ResourceType::ConstantBuffer:
        Add(Holder_, CreateConstantBuffer(Info_));
        return;
    case ResourceType::Sampler: break;
    case ResourceType::Texture:

        return;
    case ResourceType::UAV: break;
    default: ;
    }
}


void OrcaGraphics::Resource::Dx12ResourceCreator::Add(std::unordered_map<std::string, std::shared_ptr<Dx12Resource>>& Holder_,
    std::shared_ptr<Dx12Resource> pResource_)
{
    // ------------------------------- リソースを追加する -------------------------------
    Holder_.try_emplace(pResource_->GetName(), pResource_);
}

std::shared_ptr<OrcaGraphics::Resource::ConstantBuffer> OrcaGraphics::Resource::Dx12ResourceCreator::CreateConstantBuffer(
    const Dx12ResourceInfo& Info_)
{
    // ------------------------------- 定数バッファを作成 -------------------------------
    return std::make_shared<ConstantBuffer>(Info_.mBufferSize, Info_.mRootIndex);
}
