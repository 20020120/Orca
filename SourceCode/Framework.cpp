#include"Framework.h"

#include "Camera.h"
#include "LogWindow.h"
#include"GraphicsForGameLoop.h"
#include"DescriptorPool.h"
#include"ShaderDesc.h"
#include"Transform.h"
#include"ObjRenderer.h"
#include"ObjMesh.h"


// -------------------------------- �V�X�e�����C���N���[�h --------------------------------
#include"RendererSystem.h"

#include"ImGuiSetting.h"
#include"../Imgui/imgui.h"

#include<sstream>

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

    // ------------------------------- ImGui�������� -------------------------------
    ImGuiSetting::Renderer::CreateImGui(mHwnd, OrcaGraphics::GraphicsForGameLoop::GetDevice().Get(),
        OrcaGraphics::GraphicsForGameLoop::GetDescriptorPool(OrcaGraphics::POOL_TYPE_RES));

    mpCamera->Initialize();
    
    const auto gameObject = mGameObjects.AddGameObject("Test");
    const auto child = gameObject->AddChildObject("Child");

    gameObject->AddComponent<Component::Transform>();
    gameObject->AddComponent<Component::ObjMesh>(L"../Resource/Obj/Bison/Bison.obj");
    gameObject->AddComponent<Component::ObjRenderer>();

    child->AddComponent<Component::Transform>();
    child->AddComponent<Component::ObjMesh>(L"../Resource/Obj/Bison/Bison.obj");
    child->AddComponent<Component::ObjRenderer>();

    OrcaGraphics::Shader::ShaderDesc shaderDesc{};
    shaderDesc.mVsFileName = L"../Resource/Shader/ObjVs.cso";
    shaderDesc.mPsFileName = L"../Resource/Shader/ObjPs.cso";
    shaderDesc.mShaderType = OrcaGraphics::Shader::ShaderType::Sample;
    shaderDesc.mBlendState = OrcaGraphics::PipelineTypes::BlendState::Sample;
    shaderDesc.mRasterizerState = OrcaGraphics::PipelineTypes::RasterizerState::Sample;
    shaderDesc.mDepthStencilState = OrcaGraphics::PipelineTypes::DepthStencilState::Sample;
    mpPipeline = std::make_unique<OrcaGraphics::RenderPipeline>(shaderDesc);
    return true;
}

void FrameWork::Update(float Dt_)
{
    ImGuiSetting::Renderer::NewFrame();

    // �J�����s����X�V
    mpCamera->Update(Dt_);

    mGameObjects.Update(Dt_);

    // -------------------------------- �V�X�e�����X�V --------------------------------
    System::RenderSystem::Instance().Update(Dt_);

    // ----------------------------- ImGui�̃��j���[���X�V -----------------------------
    GuiMenu(Dt_);
}

void FrameWork::GuiMenu(float Dt_)
{
    mGameObjects.GuiMenu(Dt_);
}

void FrameWork::Render(float Dt_)
{
    // �R�}���h���X�g���擾
    const auto cmdList = OrcaGraphics::Graphics::GetCmdList();

    // �R�}���h���X�g�J��
    OrcaGraphics::GraphicsForGameLoop::OpenCmdList();
    // �R�}���h���X�g�Ƀe�X�g�R�}���h��ς�
    OrcaGraphics::GraphicsForGameLoop::StackCmdList();
    ImGuiSetting::Renderer::Render();

    // �V�F�[�_�[���Z�b�g����
    mpPipeline->StackGraphicsCmd(cmdList);
    mpCamera->StackGraphicsCmd(cmdList);
    System::RenderSystem::Instance().Render(cmdList);


    // ImGui�̓R�}���h�̍Ō�ɐςނ��ƁI
    ImGuiSetting::Renderer::RenderDrawData(cmdList.Get());
    // �R�}���h���X�g�����
    OrcaGraphics::GraphicsForGameLoop::CloseCmdList();
}

bool FrameWork::Finalize()
{
    mpCamera.reset();
    mGameObjects.Finalize();
    ImGuiSetting::Renderer::Cleanup();
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
