#pragma once
#include"Component.h"
#include"Obj.h"

#include<string>

namespace Component
{
    class ObjMesh final :public Component
    {
    public:
        explicit ObjMesh(std::wstring FilePath_);
        ~ObjMesh() override;

        void GuiMenu(float Dt_) override;
    private:
        Model::Obj mObjResource{};
        std::wstring mFilePath{};
    };
}
