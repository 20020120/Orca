#pragma once
#include"Component.h"
#include"Obj.h"
#include"ShaderType.h"
#include<string>

namespace Component
{
    class ObjMesh final :public Component
    {
    public:
        explicit ObjMesh(std::wstring FilePath_);
        ~ObjMesh() override;

        [[nodiscard]] Model::Obj& GetResource();
        void GuiMenu(float Dt_) override;
        void OnStart() override;
    private:
        Model::Obj mObjResource{};
        std::wstring mFilePath{};
        OrcaGraphics::Shader::ShaderType mShaderType{};
    };
}
