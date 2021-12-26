#pragma once
#include "imgui.h"

namespace Cheat
{
    inline static bool show_demo_window = false;
    inline static bool crabgame_open = false;

    inline static void ShowWelcomeWindow()
    {
        static auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGui::Begin("Crab Game Cheat - Welcome");
        ImGui::Text("Made by Orlando");
        ImGui::BulletText("The main menu will appear when Crab Game is opened.");
        ImGui::ColorEdit3("Color Wheel", reinterpret_cast<float*>(&clear_color));

        ImGui::Separator();
        ImGui::Checkbox("ImGui Demo Window", &show_demo_window);
        ImGui::End();
    }
}
