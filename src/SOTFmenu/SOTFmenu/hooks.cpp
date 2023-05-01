#include <Windows.h>
#include <iostream>
#include "kiero/minhook/include/MinHook.h"
#include "config.h"
#include "Globals.h"

typedef BOOL(WINAPI* hk_SetCursorPos)(int, int);
hk_SetCursorPos origSetCursorPos = NULL;

BOOL WINAPI HOOK_SetCursorPos(int X, int Y)
{
	if (Globals::Gui::showMenu) {
		return FALSE;
	}

	return origSetCursorPos(X, Y);
}

void InitCursorHook()
{
	if (MH_CreateHook(&SetCursorPos, &HOOK_SetCursorPos, reinterpret_cast<LPVOID*>(&origSetCursorPos)) != MH_OK)
	{
		std::cout << "Couldn't create hook for SetCursorPos." << '\n';
		return;
	}

	if (MH_EnableHook(&SetCursorPos) != MH_OK)
	{
		std::cout << "Couldn't enable SetCursorPos hook." << '\n';
	}
}

// God Mode
void(__fastcall* Vitals__ApplyDamage_o)(DWORD*, float, DWORD*);
void __stdcall Vitals__ApplyDamage_Hook(DWORD* __this, float value, DWORD* method) {

	if (Config::bHealth) {
		value = 0.0f;
	}
	//printf("ApplyDamage hook called!\n");
	return Vitals__ApplyDamage_o(__this, value, method);
}
// Stamina
void(__fastcall* Vitals__AdjustStamina_o)(DWORD*, float, DWORD*);
void __stdcall Vitals__AdjustStamina_Hook(DWORD* __this, float value, DWORD* method) {

	if (Config::bStamina) {
		value = 0.0f;
	}
	//printf("AdjustStamina hook called!\n");
	return Vitals__AdjustStamina_o(__this, value, method);
}
// Continuously grab the Cheats_CheatsBridge_o* address to call other in game functions, SinglePlayer only!
void(__fastcall* Cheats_o)(DWORD*, DWORD*);
void __stdcall Cheats_Hook(DWORD* __this, DWORD* method) {

	Globals::Cheats_CheatsBridge_o = *__this;
	
	return Cheats_o(__this, method);
}
// Infinite Ammo
void(__fastcall* Sons_Weapon_RangedWeapon_Ammo__Remove_o)(DWORD*, int, DWORD*);
void __stdcall Sons_Weapon_RangedWeapon_Ammo__Remove_Hook(DWORD* __this, int count, DWORD* method) {

	if (Config::bInfiniteAmmo) {
		count = 0;
	}
	//printf("AdjustStamina hook called!\n");
	return Sons_Weapon_RangedWeapon_Ammo__Remove_o(__this, count, method);
}

void CreateHacksHooks()
{
	// God Mode
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Globals::GameAssembly + Config::Offsets::ApplyDamage), &Vitals__ApplyDamage_Hook, reinterpret_cast<LPVOID*>(&Vitals__ApplyDamage_o)) != MH_OK) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Error creating ApplyDamage hook\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		printf("[+] ApplyDamage hook created\n");
	}
	if (MH_EnableHook(reinterpret_cast<LPVOID*>(Globals::GameAssembly + Config::Offsets::ApplyDamage)) != MH_OK) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Error enabling ApplyDamage hook\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		printf("[+] ApplyDamage hook enabled\n");
	}
	// Stamina
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Globals::GameAssembly + Config::Offsets::AdjustStamina), &Vitals__AdjustStamina_Hook, reinterpret_cast<LPVOID*>(&Vitals__AdjustStamina_o)) != MH_OK) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Error creating AdjustStamina hook\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		printf("[+] AdjustStamina hook created\n");
	}
	if (MH_EnableHook(reinterpret_cast<LPVOID*>(Globals::GameAssembly + Config::Offsets::AdjustStamina)) != MH_OK) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Error enabling AdjustStamina hook\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		printf("[+] AdjustStamina hook enabled\n");
	} 
	// Infinite Ammo
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Globals::GameAssembly + 0x37215E0), &Sons_Weapon_RangedWeapon_Ammo__Remove_Hook, reinterpret_cast<LPVOID*>(&Sons_Weapon_RangedWeapon_Ammo__Remove_o)) != MH_OK) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Error creating Infinite Ammo hook\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		printf("[+] Infinite Ammo hook created\n");
	}
	if (MH_EnableHook(reinterpret_cast<LPVOID*>(Globals::GameAssembly + 0x37215E0)) != MH_OK) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Error enabling Infinite Ammo hook\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		printf("[+] Infinite Ammo hook enabled\n");
	}
}

void CreateSinglePlayerHacksHooks()
{
	if (MH_CreateHook(reinterpret_cast<LPVOID*>(Globals::GameAssembly + Config::Offsets::GrabCheats_CheatsBridge_o), &Cheats_Hook, reinterpret_cast<LPVOID*>(&Cheats_o)) != MH_OK) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Error creating Grab Cheats_CheatsBridge_o value hook\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		printf("[+] Grab Cheats_CheatsBridge_o value hook created\n");
	}
	if (MH_EnableHook(reinterpret_cast<LPVOID*>(Globals::GameAssembly + Config::Offsets::GrabCheats_CheatsBridge_o)) != MH_OK) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Error enabling Grab Cheats_CheatsBridge_o value hook\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		printf("[+] Grab Cheats_CheatsBridge_o value hook enabled\n");
	}
}

// From here in game functions calling begins
typedef void* (__cdecl* _Cheats_CheatsBridge__SetGodMode)(DWORD* __this, bool onoff, DWORD* method);
_Cheats_CheatsBridge__SetGodMode Cheats_CheatsBridge__SetGodMode;

void GodMode()
{
	Cheats_CheatsBridge__SetGodMode = (_Cheats_CheatsBridge__SetGodMode)(Globals::GameAssembly + Config::Offsets::GodMode);

	if (Config::bHealthSP)
	{
		Cheats_CheatsBridge__SetGodMode(&Globals::Cheats_CheatsBridge_o, true, nullptr);
	}
	else {
		Cheats_CheatsBridge__SetGodMode(&Globals::Cheats_CheatsBridge_o, false, nullptr);
	}
}

typedef void* (__cdecl* _Cheats_CheatsBridge__SetInfiniteEnergy)(DWORD* __this, bool onoff, DWORD* method);
_Cheats_CheatsBridge__SetInfiniteEnergy Cheats_CheatsBridge__SetInfiniteEnergy;

void InfiniteStamina()
{
	Cheats_CheatsBridge__SetInfiniteEnergy = (_Cheats_CheatsBridge__SetInfiniteEnergy)(Globals::GameAssembly + Config::Offsets::InfiniteStamina);

	if (Config::bStaminaSP)
	{
		Cheats_CheatsBridge__SetInfiniteEnergy(&Globals::Cheats_CheatsBridge_o, true, nullptr);
	}
	else {
		Cheats_CheatsBridge__SetInfiniteEnergy(&Globals::Cheats_CheatsBridge_o, false, nullptr);
	}
}
