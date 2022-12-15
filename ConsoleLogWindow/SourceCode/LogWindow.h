#pragma once
#include<Windows.h>
namespace OrcaDebug
{
    // ---------------------------- ログをコンソール画面に出力する ----------------------------
    class LogWindow final 
    {
    public:
        enum Color
        {
            White = 7,
            Blue = 9,
            Green = 10,
            SkyBlue = 11,
            Red = 12,
        };

        // --------------------------------- 関数 --------------------------------
        static void OpenWindow();
        static void Write(const char* Message_, LogWindow::Color Color_ = White);

    private:
        inline static HANDLE mHandle{}; 
        // コンソール画面の初期サイズ
        static constexpr  SMALL_RECT mWindowSize = { 0, 0, 50, 25 };
    };
}

