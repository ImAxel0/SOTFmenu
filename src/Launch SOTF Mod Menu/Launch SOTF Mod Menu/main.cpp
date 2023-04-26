#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "ini.h"
#include <filesystem>
#include <shlobj_core.h>

using namespace mINI;

std::string gamePath_str;

void ReadGamePath()
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
    path += "\\SOTF\\config\\gamePath.ini";
    std::string path_str = path.string();
    const char* real_path = path_str.c_str();

    INIFile config(real_path);
    INIStructure ini;

    if (config.read(ini))
    {
        gamePath_str = ini["Game Path"].get("path");
    }
    else
    {
        ini["Game Path"]["path"] = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Sons Of The Forest\\SonsOfTheForest.exe";
        config.write(ini, true);
    }
}

int GetProcessByName(const char* lpProcessName)
{
    char lpCurrentProcessName[255];

    PROCESSENTRY32 ProcList{};
    ProcList.dwSize = sizeof(ProcList);

    const HANDLE hProcList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcList == INVALID_HANDLE_VALUE)
        return 1;

    if (!Process32First(hProcList, &ProcList))
        return 1;

    wcstombs_s(nullptr, lpCurrentProcessName, ProcList.szExeFile, 255);

    if (lstrcmpA(lpCurrentProcessName, lpProcessName) == 0)
        return ProcList.th32ProcessID;

    while (Process32Next(hProcList, &ProcList))
    {
        wcstombs_s(nullptr, lpCurrentProcessName, ProcList.szExeFile, 255);

        if (lstrcmpA(lpCurrentProcessName, lpProcessName) == 0)
            return ProcList.th32ProcessID;
    }

    return 1;
}

int main()
{
    using namespace std::chrono;

    ReadGamePath();

	char dllName[] = "SOTFmenu.dll";
	char processName[] = "SonsOfTheForest.exe";
    char lpFullDLLPath[MAX_PATH];

    ShellExecuteA(NULL, "open", gamePath_str.c_str(), NULL, NULL, SW_SHOWDEFAULT);

    printf("Waiting for game to load and 20 seconds to inject the Mod Menu (this will close itself after it)\n\n");

    std::this_thread::sleep_for(20s);

    const int dwProcessID = GetProcessByName(processName);
    if (dwProcessID == 1)
    {
        printf("Can't find SonsOfTheForest.exe process\n");
        system("pause");
        return 1;
    }

    const DWORD dwFullPathResult = GetFullPathNameA(dllName, MAX_PATH, lpFullDLLPath, nullptr);
    if (dwFullPathResult == 0)
    {
        printf("Couldn't find dll path\n");
        system("pause");
        return 1;
    }
    
    const HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
    if (hTargetProcess == INVALID_HANDLE_VALUE)
    {
        printf("Couldn't open the target process.\n");
        system("pause");
        return 1;
    }

    const LPVOID lpPathAddress = VirtualAllocEx(hTargetProcess, nullptr, lstrlenA(lpFullDLLPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (lpPathAddress == nullptr)
    {
        printf("Couldn't allocate memory in the target process.\n");
        system("pause");
        return 1;
    }

    const DWORD dwWriteResult = WriteProcessMemory(hTargetProcess, lpPathAddress, lpFullDLLPath, lstrlenA(lpFullDLLPath) + 1, nullptr);
    if (dwWriteResult == 0)
    {
        printf("Couldn't write the dll path in the target process.\n");
        system("pause");
        return 1;
    }

    const HMODULE hModule = GetModuleHandleA("kernel32.dll");
    if (hModule == INVALID_HANDLE_VALUE || hModule == nullptr)
    {
        system("pause");
        return 1;
    }

    const FARPROC lpFunctionAddress = GetProcAddress(hModule, "LoadLibraryA");
    if (lpFunctionAddress == nullptr)
    {
        printf("Couldm't get \"LoadLibraryA\" address.\n");
        system("pause");
        return 1;
    }

    const HANDLE hThreadCreationResult = CreateRemoteThread(hTargetProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)lpFunctionAddress, lpPathAddress, 0, nullptr);
    if (hThreadCreationResult == INVALID_HANDLE_VALUE)
    {
        printf("Couldn't create the thread in the target process.\n");
        system("pause");
        return 1;
    }
    
    printf("Mod Menu Injected!\n");
    return 0;
}