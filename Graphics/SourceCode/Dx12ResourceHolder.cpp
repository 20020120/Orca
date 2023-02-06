#include"pch.h"

#include"Dx12ResourceHolder.h"
#include"Dx12ResourceInfoCreator.h"
#include"Dx12ResourceCreator.h"

#include <ranges>
void OrcaGraphics::Dx12ResourceHolder::Add(const Shader::ShaderType& ShaderType_,
    const std::string& ObjectName_)
{
    // ----------------------------- オブジェクトを新規に追加する ----------------------------
    for (const auto& resources = Resource::Dx12ResourceInfoCreator::GetResourceMap(ShaderType_); 
        const auto& info : resources | std::views::values)
    {
        Resource::Dx12ResourceCreator::CreateResource(mHolder[ObjectName_], info);
    }
}

void OrcaGraphics::Dx12ResourceHolder::Finalize()
{
    // ------------------------------- リソース情報を破棄 -------------------------------
    mHolder.clear();
}
