#pragma once
#include"HighResolutionTimer.h"
#include <cstdint>
#include <Windows.h>
#include <Richedit.h>
#include<wrl.h>
#include<memory>
#include"GameObjectHolder.h"

#define  WM_DROPFILES    0x0233

using namespace Microsoft::WRL;



extern LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//extern ImWchar glyphRangesJapanese[];
CONST LPCWSTR APPLICATION_NAME{ L"Orc" };

// ---------------------------------- ÉNÉâÉXÇÃëOï˚êÈåæ ---------------------------------
namespace OrcaGraphics
{
	class Graphics;
}

namespace ComponentSystem
{
	class GameObject;
}

class FrameWork
{
public:
	HWND mHwnd{};


	explicit FrameWork(HWND Hwnd_);
	~FrameWork();

	FrameWork(const FrameWork&) = delete;
	FrameWork& operator=(const FrameWork&) = delete;
	FrameWork(FrameWork&&) noexcept = delete;
	FrameWork& operator=(FrameWork&&) noexcept = delete;

	int Run();

	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	[[nodiscard]] bool Initialize();
	void Update(float Dt_/*Elapsed seconds from last frame*/);
	void GuiMenu(float Dt_);
	void Render(float Dt_/*Elapsed seconds from last frame*/);
	bool Finalize();

private:
	HighResolutionTimer mTictoc{};
	uint32_t mFrames{ 0 };
	float mDeltaTime{ 0.0f };
	int hConsole = 0;
	std::shared_ptr<ComponentSystem::GameObject> mpGameObject{};
	

	void CalculateFrameStats();

};

