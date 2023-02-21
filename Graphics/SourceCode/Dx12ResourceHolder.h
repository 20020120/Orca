//#pragma once
//#include"ShaderType.h"
//#include"ConstantBuffer.h"
//#include"Texture.h"
//#include"Graphics.h"
//#include"MacroMemory.h"
//
//#include<string>
//#include<map>
//#include <unordered_map>
//#include<memory>
//#include<ResourceUploadBatch.h>
//struct ID3D12GraphicsCommandList;
//namespace OrcaGraphics
//{
//    namespace Resource
//    {
//        class Dx12Resource;
//    }
//    // 描画用リソースの保持クラス
//    class Dx12ResourceHolder
//    {
//        // オブジェクトの名前/変数名/リソース
//        using HolderType = std::map<uint64_t, std::unordered_map<std::string, std::shared_ptr<Resource::Dx12Resource>>>;
//    public:
//        static uint64_t Add(const Shader::ShaderType& ShaderType_);  // シェーダーのリソース情報を新規に追加する
//        static void StackGraphicsCmd(ID3D12GraphicsCommandList* pCmdList_, uint64_t Handle_);
//        static void Finalize(); // 終了処理
//        
//        template<class T>
//        static void CbMapping(uint64_t Handle_, std::string VariableName_, T** MappedPtr_);   // 定数バッファにCPUのインターフェースをマッピングする
//        /**
//         * \brief 
//         * \tparam Pair std::tuple<std::string(変数名),std::wstrnig(テクスチャのパス)>
//         * \param Handle オブジェクトごとのハンドル
//         * \param Pairs_ Pairがいっぱい
//         */
//        template<class ...Pair>
//        static void TexLoad(uint64_t Handle, Pair&& ...Pairs_);
//
//
//    private:
//        inline static HolderType mHolder{};
//        inline static uint64_t mHandler{};
//        
//        // --------------------- コンストラクタを隠して新規に生成できないようにする ---------------------
//    private:
//        Dx12ResourceHolder() = default;
//    };
//
//    template <class T>
//    void Dx12ResourceHolder::CbMapping(const uint64_t Handle_, std::string VariableName_, T** MappedPtr_)
//    {
//        const auto& resource = mHolder.at(Handle_).at(VariableName_);
//        if(const auto& cb = std::dynamic_pointer_cast<Resource::ConstantBuffer>(resource))
//        {
//            cb->Mapping(reinterpret_cast<void**>(MappedPtr_));
//        }
//    }
//
//    template <class ... Pair>
//    void Dx12ResourceHolder::TexLoad(uint64_t Handle_, Pair&&... Pairs_)
//    {
//        if(!mHolder.contains(Handle_))
//            return;
//
//        // テクスチャを作成
//        DirectX::ResourceUploadBatch batch(Graphics::GetDevice().Get());
//        batch.Begin();
//      
//        const auto& map = mHolder.at(Handle_);
//        for (const auto& [variableName,texPath] : std::initializer_list<std::tuple<std::string, std::wstring>>{std::forward<Pair>(Pairs_)...})
//        {
//            if (!map.contains(variableName))
//                continue;
//            const auto& res = map.at(variableName);
//            if (const auto tex = ORCA_DYNAMIC_CAST(OrcaGraphics::Resource::Texture,res))
//            {
//                tex->Load(texPath.c_str(), batch);
//            }
//        }
//        // コマンドを実行
//        const auto future = batch.End(Graphics::GetCommandQueue().Get());
//        future.wait();
//    }
//}
