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
    // �R���\�[���E�B���h���J��
    OrcaDebug::LogWindow::OpenWindow();
    // �`��Ǘ��N���X�̎��̂𐶐�
    mpGraphics = std::make_unique<OrcaGraphics::Graphics>(); 

    // ------------------------------ �ȉ��A�������֐����Ă� ------------------------------
    mpGraphics->Initialize(mHwnd);
    m_Obj.Initialize(mpGraphics->GetDevice(), L"../Resource/Obj/cube.obj");
    return true;
}

void FrameWork::Update(float Dt_)
{
    m_Obj.Update(Dt_);
}

void FrameWork::Render(float Dt_)
{
    // �R�}���h���X�g�J��
    mpGraphics->OpenCmdList();
    // �R�}���h���X�g�Ƀe�X�g�R�}���h��ς�
    mpGraphics->StackCmdList();
    m_Obj.StackGraphicsCmd(mpGraphics->GetCmdList());
    // �R�}���h���X�g�����
    mpGraphics->CloseCmdList();
}

bool FrameWork::Finalize()
{
    return true;
}

void FrameWork::CalculateFrameStats()
{
    if (++mFrames, mTictoc.TimeStamp() - mDeltaTime >= 1.0f)
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
