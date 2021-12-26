#pragma once
#include <string>
#include <regex>
#include <sstream>
#include "ProcessModule.h"

namespace Cheat {
    static ProcessMemory memory("Crab Game.exe");
    static ProcessModule game("GameAssembly.dll");
    static std::string cheat_status = "no cheats have been activated yet.", sig_status = "No signature has been entered";
    inline static uintptr_t ce_speed_addr{}, fast_slap_addr = 0x17cc3f1, infinite_jump_addr = 0xC78B55, anti_kb_addr = 0xAB9540, zero_gravity_addr = 0xc7953f, insta_slide_addr = 0xc7f180, godmode_addr{};
    inline std::string g_size;

    inline void ReplaceStr(std::string& subject, const std::string& search,
        const std::string& replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }

    static void InitiateCheat(LPCSTR sig, uintptr_t* cheat_addy)
    {
        const std::regex signature(sig);
        cheat_status = "Finding addresses...";
        if (std::smatch match; std::regex_search(g_size, match, signature)) {
            *cheat_addy = game + match.position(0);
            cheat_status = "Found address from signature.";
        }
        cheat_status = "None";
    }

    inline uintptr_t FindSig(LPCSTR sig)
    {
        const std::regex signature(sig);
        sig_status = "Finding sig...";
        if (std::smatch match; std::regex_search(g_size, match, signature))
            return game + match.position(0);
        return NULL;
    }

    std::string CheckSig(LPCSTR c_signature)
    {
        if (const auto sig = FindSig(c_signature); sig != 0)
        {
            std::stringstream ss;
            ss << std::hex << sig;
            sig_status = "Found Address - " + ss.str();
            return ss.str();
        }
        sig_status = "Failed to find address of signature";
        return { "Failed to find address of signature" };
    }
}
