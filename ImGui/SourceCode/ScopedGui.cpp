#include"ScopedGui.h"

#include"../ImGui/imgui.h"

OrcaGui::ScopedWindow::ScopedWindow(const char* WindowName_)
{
    ImGui::Begin(WindowName_);
}

OrcaGui::ScopedWindow::~ScopedWindow()
{
    ImGui::End();
}

