#pragma once

namespace Config
{
    inline bool bHealth = false;
    inline bool bStamina = false;
    inline bool bFullness = false;
    inline bool bHydration = false;
    inline bool bRest = false;
    inline bool bSpeed = false;
    inline bool bJump = false;
    inline bool bFallDamage = false;
    inline bool bGravity = false;
    inline bool bInfiniteAmmo = false;

    inline bool bHealthSP = false;
    inline bool bStaminaSP = false;

    namespace Value
    {
        inline float fullness = 100.0f;
        inline float hydration = 100.0f;
        inline float rest = 100.0f;
        inline float runSpeed = 5.4f;
        inline float swimSpeedMultiplier = 4.0f;
        inline float jumpHeight = 2.6f;
    }

    namespace Offsets
    {
        inline uintptr_t ApplyDamage = 0x347F3B0;
        inline uintptr_t AdjustStamina = 0x347F750;
        inline uintptr_t RemoveAmmo = 0x37215E0;
        inline uintptr_t GrabCheats_CheatsBridge_o = 0x59B820;
        inline uintptr_t GodMode = 0x37E79B0;
        inline uintptr_t InfiniteStamina = 0x37E7A10;
    }
}