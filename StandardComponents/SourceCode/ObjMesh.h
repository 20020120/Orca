#pragma once
#include"Component.h"
#include"Obj.h"
#include"ShaderType.h"
#include"Matrix.h"
#include<string>
#include<memory>
namespace OrcaGraphics
{
    namespace Resource
    {
        class ConstantBuffer;
    }
}
namespace Component
{
    class Transform;

    class ObjMesh final :public Component
    {
        struct alignas(256)CbData
        {
            Math::Matrix World{};
        };
      
    public:
        explicit ObjMesh(const std::wstring& FilePath_);
        ~ObjMesh() override;

        void Update(float Dt_) override;
        void GuiMenu(float Dt_) override;
        void OnStart() override;

        // -------------------------------- ゲッター -------------------------------
        [[nodiscard]] uint32_t GetDescriptorIndex()const;
        [[nodiscard]] Model::Obj& GetResource();
    private:
        // ----------------------------- 描画に必要なリソース ----------------------------
        std::unique_ptr<OrcaGraphics::Resource::ConstantBuffer> mpConstantBuffer{};
        CbData* mpCbData{};
        Model::Obj mObjResource{};
        // ------------------------------ オプション変数 ------------------------------
        std::wstring mFilePath{};
        OrcaGraphics::Shader::ShaderType mShaderType{};
        // --------------------------- キャッシュするコンポーネント --------------------------
        std::weak_ptr<Transform> mpTransform{};
    };
}
