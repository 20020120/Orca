#pragma once
#include"ShaderType.h"
#include<string>
#include<map>
#include <unordered_map>
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
        // オブジェクトの名前/変数名/リソース
        using HolderType = std::map<std::string, std::unordered_map<std::string, std::shared_ptr<Resource::Dx12Resource>>>;
    public:
        template<class T>
        static void CbMapping(std::string ObjectName_, std::string VariableName_, T* MappedPtr_);   // 定数バッファにCPUのインターフェースをマッピングする

        static void Add(const Shader::ShaderType& ShaderType_,
            const std::string& ObjectName_);  // シェーダーのリソース情報を新規に追加する
        
    private:
        inline static HolderType mHolder{};
        
        // --------------------- コンストラクタを隠して新規に生成できないようにする ---------------------
    private:
        Dx12ResourceHolder() = default;

    };
}
