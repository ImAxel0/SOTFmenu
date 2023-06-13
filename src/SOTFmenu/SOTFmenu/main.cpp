#include "includes.h"
#include "config.h"
#include "Globals.h"
#include "hooks.h"
#include "misc.h"
#include "memory.h"
#include "gui.h"
#include <string>

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	ReadConfig(); // read config.ini file
	if (Globals::Gui::ShowTerminal)
	{
		AllocConsole();
		FILE* f;
		freopen_s(&f, "CONOUT$", "w", stdout);
	}
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	Globals::Gui::hConsole = hCons;

	printf("Trying to Kill conflicting Programs if present...\n");

	killProcessByName("MSIAfterburner.exe");
	printf("[*] Tried to Kill MSIAfterburner.exe!\n");

	killProcessByName("RTSS.exe");
	printf("[*] Tried to Kill RTSS.exe!\n");
	killProcessByName("RTSSHooksLoader.exe");
	killProcessByName("RTSSHooksLoader64.exe");
	printf("[*] Tried to Kill RTSSHooksLoader/64.exe!\n\n");

	SetConsoleTextAttribute(Globals::Gui::hConsole, 15);
	printf("Initializing IL2CPP...\n");
	Globals::il2cppStatus = IL2CPP::Initialize(true);
	if (!Globals::il2cppStatus) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Can't Initialize IL2CPP!\n\n");
		exit(1);
	}
	SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
	printf("[+] IL2CPP Initialized!\n\n");

	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			SetConsoleTextAttribute(Globals::Gui::hConsole, 15);
			printf("Initializing MinHook...\n");
			MH_Initialize();
			SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
			printf("[+] MinHook Initialized!\n\n");

			findInMemory();
			while (Globals::LocalPlayer == 0 || Globals::SunLight == 0) {
				Sleep(500);
				findInMemory();
			}
			CreateHacksHooks();
			IL2CPP::Callback::Initialize();
			InstantiateMethods();
			kiero::bind(8, (void**)&Globals::Gui::oPresent, hkPresent);

			SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
			printf("\n[+] Menu Loaded!\n");
			InitCursorHook();

			SetConsoleTextAttribute(Globals::Gui::hConsole, 14);
			printf("ALL DONE!\n\n");
			SetConsoleTextAttribute(Globals::Gui::hConsole, 15);

			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}