#pragma once

#include <Windows.h>
#include "il2cppresolver/Il2CppResolver.hpp"
#include <string>

#include "includes.h"

namespace Globals
{
    inline uintptr_t GameAssembly{ NULL };
    inline Unity::CGameObject* LocalPlayer;
    inline Unity::CComponent* Vitals;
    inline Unity::CComponent* FirstPersonCharacter;
    
    namespace Stats
    {
        inline Unity::CObject* Health;
        inline Unity::CObject* Stamina;
        inline Unity::CObject* Fullness;
        inline Unity::CObject* Hydration;
        inline Unity::CObject* Rest;
    }

    namespace ToggleStats
    {
        inline bool toggleHealth = true;
        inline bool toggleStamina = true;
        inline bool toggleFullness = true;
        inline bool toggleHydration = true;
        inline bool toggleRest = true;
        inline bool toggleSpeed = true;
        inline bool toggleJumpHeight = true;
    }

    namespace Gui
    {
        inline Present oPresent;

        inline bool isGodMode = false;
        inline bool isStamina = false;
        inline bool isFullness = false;
        inline bool isHydration = false;
        inline bool isRest = false;

        inline bool showMenu = false;
        inline bool isOpen = true;
        inline std::string window = "home";
        inline bool showAbout = false;
        inline HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        inline int dot{ 1 };
        inline bool CustomTheme = false;
        inline bool RainbowTheme = false;
        inline bool RainbowThread = true;
        inline float RainbowDelay{ 0.5f };

        inline int OpenKey{ 0x50 };
        inline bool ShowTerminal = true;
    }

    inline bool il2cppStatus = false;
}