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
    inline Unity::CComponent* PlayerHangGliderAction;
    inline Unity::CComponent* CompactPistolWeaponController;
    inline Unity::CComponent* PlayerKnightVAction;
    inline Unity::CComponent* KnightVControlDefinition;
    inline Unity::CComponent* FlashlightController;
    inline Unity::CComponent* PlasmaLighterController;
    inline Unity::CComponent* LocalPlayerPlasmaLight;

    inline DWORD Cheats_CheatsBridge_o{};

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
        inline bool toggleFullness = true;
        inline bool toggleHydration = true;
        inline bool toggleRest = true;
        inline bool toggleSpeed = true;
        inline bool toggleJumpHeight = true;
    }

    namespace Gui
    {
        inline Present oPresent;

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
        inline ImVec4 LighterColor(0, 1, 0, 1);

        inline bool showMap = false;

        inline int OpenKey{ 0x50 };
        inline bool ShowTerminal = true;
    }

    inline bool initSingleHooks = false;
    inline bool il2cppStatus = false;
}