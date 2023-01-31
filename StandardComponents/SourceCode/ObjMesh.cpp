#include"ObjMesh.h"

#include <utility>
#include"Obj.h"
#include"GuiInclude.h"
Component::ObjMesh::ObjMesh(std::wstring FilePath_)
    :Component(0)
{
    mFilePath = std::move(FilePath_);
    mObjResource.Initialize(mFilePath.c_str());
}

Component::ObjMesh::~ObjMesh() = default;

Model::Obj& Component::ObjMesh::GetResource() 
{
    return mObjResource;
}

void Component::ObjMesh::GuiMenu(float Dt_)
{
    if (ImGui::TreeNode("Mesh"))
    {
        ImGui::TreePop();
    }
}
