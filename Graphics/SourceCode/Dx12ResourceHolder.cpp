#include"pch.h"

#include"Dx12ResourceHolder.h"
#include"Dx12ResourceInfoCreator.h"
#include"Dx12ResourceCreator.h"

#include <ranges>
void OrcaGraphics::Dx12ResourceHolder::StackGraphicsCmd(ID3D12GraphicsCommandList* pCmdList_, uint64_t Handle_)
{
    if (!mHolder.contains(Handle_))
        return;
    for (const auto& map = mHolder.at(Handle_); 
        auto& res : map | std::views::values)
    {
        res->Bind(pCmdList_);
    }
}

uint64_t OrcaGraphics::Dx12ResourceHolder::Add(const Shader::ShaderType& ShaderType_)
{
    // ----------------------------- �I�u�W�F�N�g��V�K�ɒǉ����� ----------------------------
    for (const auto& resources = Resource::Dx12ResourceInfoCreator::GetResourceMap(ShaderType_);
        const auto & info : resources | std::views::values)
    {
        if(info.mName=="Camera")
            continue;
        Resource::Dx12ResourceCreator::CreateResource(mHolder[mHandler], info);
    }
    return mHandler++;
}

void OrcaGraphics::Dx12ResourceHolder::Finalize()
{
    // ------------------------------- ���\�[�X����j�� -------------------------------
    mHolder.clear();
}

