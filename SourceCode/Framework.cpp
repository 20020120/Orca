#include<sstream>
#include"Framework.h"
#include "LogWindow.h"
#include"Graphics.h"

FrameWork::FrameWork(HWND Hwnd_)
    :mHwnd(Hwnd_)
{}

FrameWork::~FrameWork() = default;

int FrameWork::Run()
{
    MSG msg{};

    if (!Initialize())
    {
        return 0;
    }


    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            mTictoc.Tick();
            CalculateFrameStats();
            const auto deltaTime = mTictoc.TimeInterval();

            if (const auto hwnd = GetForegroundWindow(); mHwnd != hwnd)
            {
                //continue;
            }

            Update(deltaTime);
            Render(mTictoc.TimeInterval());
        }
    }
    return Finalize() ? static_cast<int>(msg.wParam) : 0;
}

LRESULT FrameWork::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    //if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }

    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps{};
        BeginPaint(hwnd, &ps);

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        break;
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE)
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        break;
    case WM_ENTERSIZEMOVE:
        mTictoc.Stop();
        break;
    case WM_EXITSIZEMOVE:
        mTictoc.Start();
        break;

    case WM_MOUSEHWHEEL:
    {
       
    }
    break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

bool FrameWork::Initialize()
{
    // コンソールウィンドを開く
    OrcaDebug::LogWindow::OpenWindow();
    // 描画管理クラスの実体を生成
    mpGraphics = std::make_unique<OrcaGraphics::Graphics>(); 



    // ------------------------------ 以下、初期化関数を呼ぶ ------------------------------
    mpGraphics->Initialize(mHwnd);


    return true;
}

void FrameWork::Update(float Dt_)
{

}

void FrameWork::Render(float Dt_) const
{
    mpGraphics->Render();
}

bool FrameWork::Finalize()
{
    return true;
}

void FrameWork::CalculateFrameStats()
{
    if (++mFrames, (mTictoc.TimeStamp() - mDeltaTime) >= 1.0f)
    {
        const auto fps = static_cast<float>(mFrames);
        std::wostringstream outs;
        outs.precision(6);
        outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)" << "\n";
        auto a = SetWindowTextW(mHwnd, outs.str().c_str());

        OutputDebugStringW(outs.str().c_str());

        mFrames = 0;
        mDeltaTime += 1.0f;

    }
}
