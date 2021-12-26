#pragma once
#include "CheatMenu.hpp"
#include <TlHelp32.h>

namespace Cheat
{
    const char* const crabgame_procname = "Crab Game.exe";

    inline void InitMemory()
    {
        if (const auto result = memory.open(); result != ProcessStatus::Ok)
            cheat_status = "Unable to open process";

        if (const auto result = game.open(memory); result != ProcessStatus::Ok)
            cheat_status = "Unable to open module";

        g_size.resize(game.size);
        if (const auto result = memory.read(game, game.size, &g_size[0]); result != ProcessStatus::Ok)
            cheat_status = "Unable to read image";
    }

    inline static void CrabGameOpen()
    {
        bool found_crabgame = false;
        static bool ran_sig_check = false;
        HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        PROCESSENTRY32 pEntry{};
        pEntry.dwSize = sizeof pEntry;
        BOOL hRes = Process32First(hSnapShot, &pEntry);
        while (hRes)
        {
            if (strcmp(pEntry.szExeFile, crabgame_procname) == 0)
            {
                if (HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pEntry.th32ProcessID); hProcess != nullptr) {
                    if (!crabgame_open)
                        InitMemory();
                    //open_info_popup = false, show_sig_window = false, show_stack_tool = false, ce_speed_bypass = false, fast_slap = false, infinite_jump = false, anti_kb = false, zero_gravity = false, insta_slide = false, godmode = false;

                    if (!ran_sig_check) {
                        InitiateCheat(ce_speed_sig, &ce_speed_addr);
                        InitiateCheat(godmode_sig, &godmode_addr);
                        ran_sig_check = true;
                    }
                    found_crabgame = true;
                }
            }
            hRes = Process32Next(hSnapShot, &pEntry);
        }
        CloseHandle(hSnapShot);
        if (found_crabgame)
            crabgame_open = true;
        else
            crabgame_open = false;
        Sleep(100);
    }

    [[noreturn]]
    inline static void CrabGameChecker()
    {
        while (true)
        {
            CrabGameOpen();
            Sleep(200);
        }
    }
}
