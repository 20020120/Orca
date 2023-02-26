#pragma once
#include<string>
// ファイルの読み書きのダイアログ
namespace WinApi
{
    
    struct FileDialog
    {
        static bool OpenFileNameA(char* FileName_, int Size_, const char* iniDir_, const char* Filter_);
        static bool SaveFileNameA(char* FileName_);
        static bool GetFolderNameA(std::string& FileName_);
    };
}
