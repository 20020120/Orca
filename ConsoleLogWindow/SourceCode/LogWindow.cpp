#include"pch.h"
#include"LogWindow.h"
#include"LogDefine.h"

#include<cstdio>
#include<iostream>
void OrcaDebug::LogWindow::OpenWindow()
{
    // ------------------------------ ���O�E�C���h�E���J���� ------------------------------
#ifdef OpenConsoleWind 
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    setvbuf(stdout, nullptr, _IONBF, 0);
    mHandle = GetStdHandle(STD_OUTPUT_HANDLE);	// DWORD nStdHandle
    if (mHandle != INVALID_HANDLE_VALUE)
    {
        SetConsoleWindowInfo(
            mHandle, // HANDLE hConsoleOutput
            TRUE,           // BOOL bAbsolute
            &LogWindow::mWindowSize);// CONST SMALL_RECT *lpConsoleWindow
    }
    // �R���\�[����������Ƃ��Ɉُ�I������̂��������p
    SetConsoleCtrlHandler([](DWORD)->BOOL {return true; }, TRUE);
#endif   
}

void OrcaDebug::LogWindow::Write(const char* Message_, LogWindow::Color Color_)
{
#ifdef OpenConsoleWind
    SetConsoleTextAttribute(mHandle, Color_);
    std::cout << Message_ << std::endl;
    SetConsoleTextAttribute(mHandle, Color::White);
#endif
}

