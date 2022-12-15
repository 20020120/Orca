#pragma once
#include<Windows.h>
namespace OrcaDebug
{
    // ---------------------------- ���O���R���\�[����ʂɏo�͂��� ----------------------------
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

        // --------------------------------- �֐� --------------------------------
        static void OpenWindow();
        static void Write(const char* Message_, LogWindow::Color Color_ = White);

    private:
        inline static HANDLE mHandle{}; 
        // �R���\�[����ʂ̏����T�C�Y
        static constexpr  SMALL_RECT mWindowSize = { 0, 0, 50, 25 };
    };
}

