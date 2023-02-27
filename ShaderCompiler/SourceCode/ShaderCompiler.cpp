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
        // ------------------------------ �ݒ�t�@�C�����J�� ------------------------------
        if (exists(settingPath))
        {
            // ------------------------- �J������ŏI�X�V�������擾 -------------------------
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
    // ---------------------------- �t�@�C���̏ꏊ�����݂��Ȃ������� ---------------------------
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
    // --------------------------- ����f�B���N�g�����̑S�t�@�C���𑖍� --------------------------
    using namespace std::filesystem;
    directory_iterator iter(folderPath);
    const directory_iterator end;
    std::error_code err;

    for (; iter != end && !err; iter.increment(err)) {
        const directory_entry entry = *iter;

        // HLSL�t�@�C�������𒊏o
        if (entry.path().extension() == ".hlsl")
        {
            file_names.push_back(entry.path().string());
            printf("%s\n", file_names.back().c_str());
        }
    }

    // �R���p�C���ƃ��[�e�B���e�B�𐶐�
    Microsoft::WRL::ComPtr<IDxcUtils> pUtils;
    Microsoft::WRL::ComPtr<IDxcCompiler3> pCompiler;
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

    // �f�t�H���g�C���N���[�h�n���h�����쐬
    Microsoft::WRL::ComPtr < IDxcIncludeHandler> pIncludeHandler;
    pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

    // �V�F�[�_�[�̎�ނɉ����ăR���p�C������
    for (const auto& path : file_names)
    {
        // ----------------------- �t�@�C�������݂��Ă��Ȃ���ΕK���R���p�C�� -----------------------
        {
            std::filesystem::path p = path;
            p = p.replace_extension("bin");
            std::filesystem::path o = outDir;
            auto outName = o.append(p.filename().c_str());
            if (std::filesystem::exists(o))
            {
                // --------------------- �t�@�C���̍X�V��������R���p�C�����邩�ǂ������m�F ---------------------
                if (auto fileWriteTime = last_write_time(path); lastCompileTime > fileWriteTime)
                {
                    continue;
                }
            }
        }
        // ------------------------------- ���_�V�F�[�_ ------------------------------
        if (path.find("Vs") != std::string::npos)
        {
            DXC dxc{};
            dxc.Compile(pUtils, pCompiler, pIncludeHandler, Util::Strings::StringToWString(path).c_str(), L"vs_6_6", outDir.c_str());
            continue;
        }
        // ------------------------------ �s�N�Z���V�F�[�_ -----------------------------
        if (path.find("Ps") != std::string::npos)
        {
            DXC dxc{};
            dxc.Compile(pUtils, pCompiler, pIncludeHandler, Util::Strings::StringToWString(path).c_str(), L"ps_6_6", outDir.c_str());
            continue;
        }

        // ------------------------------ �G���[���b�Z�[�W -----------------------------
        std::cout << "FAILED!!_____Shader Stage Not Found!!" << std::endl;
    }

}
