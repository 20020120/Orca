#include"pch.h"
#include"GraphicsLogger.h"

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <system_error>
#include <wrl.h>

void OrcaDebug::GraphicsLog(const char* Message_, long Hr_)
{
    // ƒƒO‚ğo—Í‚·‚é
    std::cout << Message_ << std::endl << std::system_category().message(Hr_) << std::endl;

    // ‰Šú‰»‚É¸”s‚µ‚Ä‚¢‚½‚ç‹­§I—¹
    if (FAILED(Hr_))
    {
        throw std::logic_error(Message_);
    }

}
