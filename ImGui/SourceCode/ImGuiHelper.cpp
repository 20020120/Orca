#include"ImGuiHelper.h"
#include"../ImGui/imgui.h"

void OrcaGui::MainMenu(const char* Label_, const char* Detail_, bool* IsOpen_)
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu(Label_))
    {
        ImGui::MenuItem(Detail_, nullptr, IsOpen_);
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}
