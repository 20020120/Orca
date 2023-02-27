#include"ShaderCompiler.h"
#include"StringUtil.h"

#include<filesystem>
#include<iostream>
#include<fstream>
#include<wrl.h>

#include<d3d12shader.h>
#include<dxcapi.h>

#include "DXC.h"

void ShaderCompiler::AllCompile(const char* HLSLDir_, const char* OutDir_, const char* SettingFileDir_)
{
    std::filesystem::file_time_type lastCompileTime = std::filesystem::file_time_type::clock::now();
    {
        std::filesystem::path settingPath = SettingFileDir_;
        settingPath.replace_filename("Setting.txt");
        // ------------------------------ 設定ファイルを開く ------------------------------
        if (exists(settingPath))
        {
            // ------------------------- 開けたら最終更新日時を取得 -------------------------
            lastCompileTime = last_write_time(settingPath);
        }
        char dummyStr{ 'a' };
        std::ofstream fOut;
        fOut.open(settingPath, std::ios::out | std::ios::binary | std::ios::trunc);
        fOut.write(&dummyStr, 1);
        fOut.close();
    }
    std::cout << lastCompileTime << std::endl;

    const std::string folderPath = HLSLDir_;
    const std::wstring outDir = Util::Strings::StringToWString(OutDir_);

    std::wcout << outDir.c_str() << std::endl;
    // ---------------------------- ファイルの場所が存在しなかったら ---------------------------
    if (!std::filesystem::exists(folderPath))
    {
        std::cout << "FAILED!!_____FolderPath Not Found!!" << std::endl;
        return ;
    }
    if (!std::filesystem::exists(outDir))
    {
        std::cout << "FAILED!!_____OutDir Not Found!!" << std::endl;
        return ;
    }

    std::vector<std::string> file_names{};
    // --------------------------- 特定ディレクトリ内の全ファイルを走査 --------------------------
    using namespace std::filesystem;
    directory_iterator iter(folderPath);
    const directory_iterator end;
    std::error_code err;

    for (; iter != end && !err; iter.increment(err)) {
        const directory_entry entry = *iter;

        // HLSLファイルだけを抽出
        if (entry.path().extension() == ".hlsl")
        {
            file_names.push_back(entry.path().string());
            printf("%s\n", file_names.back().c_str());
        }
    }

    // コンパイラとユーティリティを生成
    Microsoft::WRL::ComPtr<IDxcUtils> pUtils;
    Microsoft::WRL::ComPtr<IDxcCompiler3> pCompiler;
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

    // デフォルトインクルードハンドラを作成
    Microsoft::WRL::ComPtr < IDxcIncludeHandler> pIncludeHandler;
    pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

    // シェーダーの種類に応じてコンパイルする
    for (const auto& path : file_names)
    {
        // ----------------------- ファイルが存在していなければ必ずコンパイル -----------------------
        {
            std::filesystem::path p = path;
            p = p.replace_extension("bin");
            std::filesystem::path o = outDir;
            auto outName = o.append(p.filename().c_str());
            if (std::filesystem::exists(o))
            {
                // --------------------- ファイルの更新日時からコンパイルするかどうかを確認 ---------------------
                if (auto fileWriteTime = last_write_time(path); lastCompileTime > fileWriteTime)
                {
                    continue;
                }
            }
        }
        // ------------------------------- 頂点シェーダ ------------------------------
        if (path.find("Vs") != std::string::npos)
        {
            DXC dxc{};
            dxc.Compile(pUtils, pCompiler, pIncludeHandler, Util::Strings::StringToWString(path).c_str(), L"vs_6_6", outDir.c_str());
            continue;
        }
        // ------------------------------ ピクセルシェーダ -----------------------------
        if (path.find("Ps") != std::string::npos)
        {
            DXC dxc{};
            dxc.Compile(pUtils, pCompiler, pIncludeHandler, Util::Strings::StringToWString(path).c_str(), L"ps_6_6", outDir.c_str());
            continue;
        }

        // ------------------------------ エラーメッセージ -----------------------------
        std::cout << "FAILED!!_____Shader Stage Not Found!!" << std::endl;
    }

}
