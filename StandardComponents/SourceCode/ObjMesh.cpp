#include"ObjMesh.h"
#include"Obj.h"
#include"GuiInclude.h"
Component::ObjMesh::ObjMesh(std::wstring FilePath_)
    :Component(0)
{
    mFilePath = FilePath_;
    mObjResource.Initialize(FilePath_.c_str());
}

Component::ObjMesh::~ObjMesh()
{}
void Component::ObjMesh::GuiMenu(float Dt_)
{
    if (ImGui::TreeNode("Mesh"))
    {
        ImGui::TreePop();
    }
}