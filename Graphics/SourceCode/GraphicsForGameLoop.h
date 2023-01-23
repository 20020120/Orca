#pragma once

#include"Graphics.h"

#include<Windows.h>
namespace OrcaGraphics
{
    class GraphicsForGameLoop final :public Graphics
    {
    public:
        static void Initialize(HWND Hwnd_); // 初期化
        static void Finalize(); // 終了処理

        static void OpenCmdList();
        static void CloseCmdList();
        static void StackCmdList();
    };
}
