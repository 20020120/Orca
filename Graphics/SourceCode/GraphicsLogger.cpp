#include"pch.h"
#include"GraphicsLogger.h"

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <system_error>

void OrcaDebug::GraphicsLog(long Hr_)
{
    // ƒƒO‚ğo—Í‚·‚é
    std::cout << std::system_category().message(Hr_);
}
