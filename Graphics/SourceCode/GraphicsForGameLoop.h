#pragma once

#include"Graphics.h"

#include<Windows.h>
namespace Graphics
{
    class GraphicsForGameLoop final :public Graphics
    {
    public:
        static void Initialize(HWND Hwnd_); // ������
        static void Finalize(); // �I������

        static void OpenCmdList();
        static void CloseCmdList();
        static void StackCmdList();
    };
}
