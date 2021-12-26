#pragma once
#include "CheatWelcome.hpp"
#include "CheatSigs.h"
#include "CheatUtility.hpp"
#include <thread>

namespace Cheat
{
    inline const char* const my_github_link = "https://github.com/dehoisted";
    inline const char* const my_project_link = "https://github.com/dehoisted/CrabGame-Cheat-Menu";
    inline static bool open_info_popup = false, show_sig_window = false, show_stack_tool = false, ce_speed_bypass = false, fast_slap = false, infinite_jump = false, anti_kb = false, zero_gravity = false, insta_slide = false, godmode = false;
    inline const char* const sig_help_link = "https://www.unknowncheats.me/forum/programming-for-beginners/171994-understanding-pattern-scanning-concept.html";

    static void ToggleCheat(const uintptr_t address, const LPCSTR new_bytes,
        const std::size_t new_bytes_size, const std::string& cheat_name, bool* cheat)
    {
        if (const ProcessStatus write_result = memory.write(game.address + address, new_bytes_size, new_bytes); write_result == ProcessStatus::Ok)
        {
            if (!*cheat)
            {
                cheat_status = "Turned on " + cheat_name + " successfully.";
                *cheat = true;
            }
            else
            {
                cheat_status = "Turned off " + cheat_name + " successfully.";
                *cheat = false;
            }
        }
        else
        {
            if (!*cheat)
                cheat_status = "Unable to turn off " + cheat_name + '.';
            else
                cheat_status = "Unable to turn on " + cheat_name + '.';
            *cheat = false;
        }
    }

    inline static void ShowSignatureWindow()
    {
        static char c_signature[128 * 5] = "";
        static long long sig_size = 0;

        ImGui::Begin("Crab Game Cheat - Signature Menu", &show_sig_window);
        ImGui::Text("Enter a code-type byte signature");
        ImGui::SameLine();
        if (ImGui::SmallButton("Copy Link For Info"))
            ImGui::SetClipboardText(sig_help_link);

        ImGui::BulletText("Only supports sigs from GameAssembly.dll");
        ImGui::BulletText(R"(Signature Example: \x4d\x5a\x90)");
        ImGui::InputTextWithHint("##sig_input", "<signature here>", c_signature, IM_ARRAYSIZE(c_signature), ImGuiInputTextFlags_CharsNoBlank);

        ImGui::Separator();
        ImGui::Text("Sig Status: %s", sig_status.c_str());
        ImGui::Text("Sig Size: %llu", sig_size);

        ImGui::Separator();
        if (ImGui::Button("Get Sig Address & Info"))
        {
            std::string s_signature = c_signature;
            if (std::size_t found_backslash = s_signature.find('\\'); found_backslash != std::string::npos)
            {
                sig_size = std::count(s_signature.begin(), s_signature.end(), '\\');
                if (sig_size >= 3)
                    std::thread(CheckSig, c_signature).detach();
                else
                    sig_status = "Sig must have a size of 3 or above";
            }
            else
                sig_status = "A code signature must have a backslash";
        }

        if (ImGui::Button("Copy Address To Clipboard"))
            ImGui::SetClipboardText(CheckSig(c_signature).c_str());

        if (ImGui::Button("Copy Regex To Clipboard"))
        {
            std::string regex_signature = c_signature;
            ReplaceStr(regex_signature, "\\x00", ".");
            ImGui::SetClipboardText(regex_signature.c_str());
        }
        ImGui::End();
    }

    inline static void ShowMenuBars()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Extra"))
            {
                if (ImGui::MenuItem("Program Info"))
                    open_info_popup = true;

                ImGui::MenuItem("Signature Menu", NULL, &show_sig_window);
                ImGui::MenuItem("Stack Tool", NULL, &show_stack_tool);

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (open_info_popup)
        {
            ImGui::OpenPopup("Program Info");
            open_info_popup = false;
        }

        if (ImGui::BeginPopupModal("Program Info"))
        {
            ImGui::TextColored(ImVec4(0.91f, 0.64f, 0.13f, 1.00f), my_project_link);
            ImGui::BulletText("Creator: Orlando/Dehoisted");
            ImGui::BulletText("Language Used: C++ (17)");
            ImGui::BulletText("GUI: GLFW (OpenGL) & ImGui");
            ImGui::Text("Last Update: 12/23/21");
            ImGui::Separator();
            if (ImGui::Button("Close Window"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        if (show_sig_window)
            ShowSignatureWindow();

        if (show_stack_tool)
            ImGui::ShowStackToolWindow(&show_stack_tool);
    }

    inline static void HelpMarker(const char* text, const char* label = "(?)")
    {
        ImGui::TextDisabled(label);
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(text);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    inline static void CheatOptionsEvts()
    {
        ImGui::Text("Cheat Options");
        if (ImGui::Button("CE Speed Hack Bypass"))
        {
            if (!ce_speed_bypass) {
                if (const ProcessStatus write_result = memory.write(ce_speed_addr, 2, "\xC3\x90"); write_result == ProcessStatus::Ok)
                {
                    cheat_status = "Turned on CE Speed Bypass successfully.";
                    ce_speed_bypass = true;
                }

                else
                    cheat_status = "Unable to turn on CE Speed Bypass.";
            }

            else
            {
                if (const ProcessStatus write_result = memory.write(ce_speed_addr, 2, "\x40\x53"); write_result == ProcessStatus::Ok) {
                    cheat_status = "Turned off CE Speed Bypass successfully.";
                    ce_speed_bypass = false;
                }
                else
                    cheat_status = "Unable to turn off CE Speed Bypass.";
            }
        }
        ImGui::SameLine();
        HelpMarker("First toggle bypass, then use CheatEngine SpeedHack.");

        if (ImGui::Button("Fast Slap"))
        {
            if (!fast_slap)
                ToggleCheat(fast_slap_addr, "\x90\x90\x90\x90\x90\x90\x90", 7, "Fast Slap", &fast_slap);
            else
                ToggleCheat(fast_slap_addr, "\x80\x7b\x20\x00\x74\x66\xf3", 7, "Fast Slap", &fast_slap);
        }

        if (ImGui::Button("Infinite Jump"))
        {
            if (!infinite_jump)
                ToggleCheat(infinite_jump_addr, "\x90\x90\x90\x90\x90\x90\x90\x90\x90", 9, "Infinite Jump", &infinite_jump);
            else
                ToggleCheat(infinite_jump_addr, "\x80\xbb\x04\x01\x00\x00\x00\x74\x09", 9, "Infinite Jump", &infinite_jump);
        }
        if (ImGui::Button("Anti KB"))
        {
            if (!anti_kb)
                ToggleCheat(anti_kb_addr, "\xC3\x90\x90\x90\x90", 5, "Anti KB", &anti_kb);
            else
                ToggleCheat(anti_kb_addr, "\x48\x89\x5c\x24\x08", 5, "Anti KB", &anti_kb);
        }
        ImGui::SameLine();
        HelpMarker("Anti KB may not work in some cases.");

        if (ImGui::Button("Zero Gravity"))
        {
            if (!zero_gravity)
                ToggleCheat(zero_gravity_addr, "\x90\x90\x90\x90\x90", 5, "Zero Gravity", &zero_gravity);
            else
                ToggleCheat(zero_gravity_addr, "\xe8\xec\x3d\xd1\xff", 5, "Zero Gravity", &zero_gravity);
        }
        if (ImGui::Button("Insta Slide"))
        {
            if (!insta_slide)
                ToggleCheat(insta_slide_addr, "\x90\x90\x90\x90\x90\x90\x90", 7, "Insta Slide", &insta_slide);
            else
                ToggleCheat(insta_slide_addr, "\xc6\x83\xcc\x01\x00\x00\x00", 7, "Insta Slide", &insta_slide);
        }
        if (ImGui::Button("Godmode"))
        {
            if (!godmode) {
                if (const ProcessStatus write_result = memory.write(godmode_addr, 6, "\x90\x90\x90\x90\x8b\x40"); write_result == ProcessStatus::Ok) {
                    cheat_status = "Turned on Godmode.";
                    godmode = true;
                }
                else
                    cheat_status = "Unable to turn on Godmode";
            }
            else {
                if (const ProcessStatus write_result = memory.write(godmode_addr, 6, "\x0f\x11\x43\x18\x8b\x40"); write_result == ProcessStatus::Ok) {
                    cheat_status = "Turned off Godmode.";
                    godmode = false;
                }
                else
                    cheat_status = "Unable to turn off Godmode";
            }
        }
        ImGui::SameLine();
        HelpMarker("Godmode may not work in some cases.");
    }

    inline static void ShowCheatStatusCFrame()
    {
        static ImVec4 on_color = { 0.41f, 0.62f, 0.15f, 1.00f }, off_color = { 1.00f, 0.20f, 0.20f, 0.90f }; /*NOTE*/ //alternative for off_color: 0.89f, 0.00f, 0.19f, 1.00f (kinda pink)
        ImGui::Separator();
        ImGui::Text("Status: %s", cheat_status.c_str());
        const auto child_size = ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 7.2f);
        ImGui::BeginChildFrame(ImGui::GetID("cheat_status"), child_size, ImGuiWindowFlags_NoMove);
        ImGui::BulletText("CE Speed Bypass: ");
        ImGui::SameLine(0.0f, 0.0f);
        ce_speed_bypass ? ImGui::TextColored(on_color, "On") : ImGui::TextColored(off_color, "Off");
        ImGui::BulletText("Fast Slap: ");
        ImGui::SameLine(0.0f, 0.0f);
        fast_slap ? ImGui::TextColored(on_color, "On") : ImGui::TextColored(off_color, "Off");
        ImGui::BulletText("Infinite Jump: ");
        ImGui::SameLine(0.0f, 0.0f);
        infinite_jump ? ImGui::TextColored(on_color, "On") : ImGui::TextColored(off_color, "Off");
        ImGui::BulletText("Anti KB: ");
        ImGui::SameLine(0.0f, 0.0f);
        anti_kb ? ImGui::TextColored(on_color, "On") : ImGui::TextColored(off_color, "Off");
        ImGui::BulletText("Zero Gravity: ");
        ImGui::SameLine(0.0f, 0.0f);
        zero_gravity ? ImGui::TextColored(on_color, "On") : ImGui::TextColored(off_color, "Off");
        ImGui::BulletText("Insta Slide: ");
        ImGui::SameLine(0.0f, 0.0f);
        insta_slide ? ImGui::TextColored(on_color, "On") : ImGui::TextColored(off_color, "Off");
        ImGui::BulletText("Godmode: ");
        ImGui::SameLine(0.0f, 0.0f);
        godmode ? ImGui::TextColored(on_color, "On") : ImGui::TextColored(off_color, "Off");
        ImGui::EndChildFrame();
    }

    inline static void ShowCheatMenuWindow()
    {
        static bool* p_open = false;
        static bool no_menu = false;

        static ImGuiWindowFlags window_flags = 0;
        if (!no_menu)
            window_flags |= ImGuiWindowFlags_MenuBar;

        ImGui::Begin("Crab Game Cheat - Menu", p_open, window_flags);
        ShowMenuBars();

        ImGui::TextColored(ImVec4(0.91f, 0.64f, 0.13f, 1.00f), "Cheat Created by Dehoisted (On Github)");
        ImGui::SameLine();
        HelpMarker(my_github_link);
        ImGui::Separator();

        CheatOptionsEvts();

        ShowCheatStatusCFrame();

        ImGui::Separator();
        if (ImGui::SmallButton("Copy Cheat Source Link To Clipboard"))
            ImGui::SetClipboardText(my_project_link);
        ImGui::Text("Application FPS: %.1f", ImGui::GetIO().Framerate + 0.1);
        ImGui::End();
    }
}
