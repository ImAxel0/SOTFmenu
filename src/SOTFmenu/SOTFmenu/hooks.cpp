#include <Windows.h>
#include <iostream>
#include "kiero/minhook/include/MinHook.h"
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