#include"pch.h"

#include"Dx12ResourceHolder.h"
#include"Dx12ResourceInfoCreator.h"
#include"Dx12ResourceCreator.h"

#include <ranges>
void OrcaGraphics::Dx12ResourceHolder::Add(const Shader::ShaderType& ShaderType_,
    const std::string& ObjectName_)
{
    // ----------------------------- オブジェクトを新規に追加する ----------------------------
    const auto& resources = Resource::Dx12ResourceInfoCreator::GetResourceMap(ShaderType_);
    for (const auto& info : resources | std::views::values)
    {
        Resource::Dx12ResourceCreator::CreateResource(mHolder[ObjectName_], info);
    }
}
