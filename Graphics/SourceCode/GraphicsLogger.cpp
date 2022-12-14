#include"pch.h"
#include"GraphicsLogger.h"

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <system_error>
#include <wrl.h>

void OrcaDebug::GraphicsLog(const char* Message_, long Hr_)
{
    // ログを出力する
    std::cout << Message_ << std::endl << std::system_category().message(Hr_) << std::endl;

    // 初期化に失敗していたら強制終了
    if (FAILED(Hr_))
    {
        throw std::logic_error(Message_);
    }

}
