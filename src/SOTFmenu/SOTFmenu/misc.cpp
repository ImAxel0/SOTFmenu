#include <Windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include "ini.h"
#include "Globals.h"
#include <filesystem>
#include <shlobj_core.h>

using namespace mINI;

void killProcessByName(const char* filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (strcmp(pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

void ReadConfig()
{
	std::filesystem::path path;
	PWSTR path_tmp;
	auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_tmp);
	// Error check
	if (get_folder_path_ret != S_OK) {
		CoTaskMemFree(path_tmp);
	}
	path = path_tmp;
	CoTaskMemFree(path_tmp);
	path += "\\SOTF\\config\\config.ini";
	std::string path_str = path.string();
	const char* real_path = path_str.c_str();

	INIFile config(real_path);
	INIStructure ini;

	if (config.read(ini))
	{
		Globals::Gui::OpenKey = std::stoi(ini["Open/close key"].get("key value"), nullptr, 16);
		Globals::Gui::ShowTerminal = std::stoi(ini["Show Terminal"].get("value"));
	}
	else
	{
		ini["Open/close key"]["key value"] = "0x50";
		ini["Show Terminal"]["value"] = "1";
		config.write(ini, true);
	}
}