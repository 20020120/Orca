#include"Framework.h"

#include "Camera.h"
#include "LogWindow.h"
#include"GraphicsForGameLoop.h"
#include"DescriptorPool.h"

#include"CharacterBuilder.h"

// -------------------------------- �V�X�e�����C���N���[�h --------------------------------
#include"RendererSystem.h"

#include"ImGuiSetting.h"
#include"../Imgui/imgui.h"
#include<sstream>
#include"FbxMesh.h"
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
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;

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
    // ------------------------------ �ȉ��A�������֐����Ă� ------------------------------
    OrcaGraphics::GraphicsForGameLoop::Initialize(mHwnd);
    System::RenderSystem::Instance().OnAwake();

    // ------------------------------- ImGui�������� -------------------------------
    ImGuiSetting::Renderer::CreateImGui(mHwnd, OrcaGraphics::GraphicsForGameLoop::GetDevice().Get(),
        OrcaGraphics::GraphicsForGameLoop::GetDescriptorPool(OrcaGraphics::POOL_TYPE_RES));

    OrcaGraphics::Camera::Instance().Initialize();

    mpGameObject = std::make_shared<ComponentSystem::GameObject>("character");
    OrcaWizard::CharacterBuilder(mpGameObject, "../Resource/Model/HunterGun1004.fbx");
    mpGameObject->OnStart();
    return true;
}

void FrameWork::Update(float Dt_)
{

    ImGuiSetting::Renderer::NewFrame();

    // �J�����s����X�V
    OrcaGraphics::Camera::Instance().Update(Dt_);

    mpGameObject->Update(Dt_);

    // -------------------------------- �V�X�e�����X�V --------------------------------
    System::RenderSystem::Instance().Update(Dt_);

    // ----------------------------- ImGui�̃��j���[���X�V -----------------------------
    GuiMenu(Dt_);
}

void FrameWork::GuiMenu(float Dt_)
{
    auto com = mpGameObject->GetComponent<Component::FbxMesh>();
    if(com)
    {
        com->EditorGuiMenu(Dt_);
    }
}

void FrameWork::Render(float Dt_)
{
    // �R�}���h���X�g���擾
    const auto cmdList = OrcaGraphics::Graphics::GetCmdList();

    // �R�}���h���X�g�J��
    OrcaGraphics::GraphicsForGameLoop::OpenCmdList();
    
    OrcaGraphics::GraphicsForGameLoop::StackCmdList();
    ImGuiSetting::Renderer::Render();

    // �V�F�[�_�[���Z�b�g����
    System::RenderSystem::Instance().Begin(cmdList);
    System::RenderSystem::Instance().Render(cmdList);


    // ImGui�̓R�}���h�̍Ō�ɐςނ��ƁI
    ImGuiSetting::Renderer::RenderDrawData(cmdList.Get());
    // �R�}���h���X�g�����
    OrcaGraphics::GraphicsForGameLoop::CloseCmdList();

}

bool FrameWork::Finalize()
{
    OrcaGraphics::Camera::Instance().Finalize();
    mpGameObject.reset();
    ImGuiSetting::Renderer::Cleanup();
    System::RenderSystem::Instance().Finalize();
    OrcaGraphics::GraphicsForGameLoop::Finalize();
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
