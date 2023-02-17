#include"Binary.h"
#include"MacroMinmam.h"
#include"OrcaException.h"
#include<filesystem>
#include<fstream>

void OrcaBinary::Load(const wchar_t* FileName_, void** pBegin_, size_t& BufferSize_)
{
    // デバッグビルドの時のみファイルの生存確認をする
    OrcaExecuteDebug(
        if (!std::filesystem::exists(FileName_))
            Orca_Assert("OrcaBinary::Load__ファイルを開くことができません"););
    if (*pBegin_)
    {
        Orca_Assert("OrcaBinary::Load__先頭アドレスが既にデータを持っています");
    }
    // ----------------------------- ファイルを開いてデータを流す ----------------------------
    std::ifstream ifs;
    ifs.open(FileName_, std::ios::binary | std::ios::in);
    // ------------------------------- ファイルサイズ取得 -------------------------------
    ifs.seekg(0, std::ios_base::end);
    BufferSize_ = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    
    *pBegin_ = new char[BufferSize_]();
    ifs.read(static_cast<char*>(*pBegin_), static_cast<std::streamsize>(BufferSize_));
    ifs.close();
}