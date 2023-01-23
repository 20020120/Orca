#pragma once
#include "Dx12ResourceInfo.h"
#include"ShaderType.h"
#include<map>
#include<memory>
namespace OrcaGraphics
{
    namespace Resource
    {
        class Dx12Resource;
    }
    // 描画用リソースの保持クラス
    class Dx12ResourceHolder
    {
        using HolderType = std::multimap<std::string, std::map<std::string, std::shared_ptr<Resource::Dx12Resource>>>;
    public:
        template<class T>
        static void CbMapping(std::string ObjectName_, std::string VariableName_, T* MappedPtr_);   // 定数バッファにCPUのインターフェースをマッピングする

        static void Add(Shader::ShaderType, std::string ObjectName_);  // シェーダーのリソース情報を新規に追加する


    private:
        inline static HolderType mHolder{};


        // --------------------- コンストラクタを隠して新規に生成できないようにする ---------------------
    private:
        Dx12ResourceHolder() = default;

    };
}
