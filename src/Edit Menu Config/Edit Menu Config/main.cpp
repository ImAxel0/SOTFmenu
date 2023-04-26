#include <Windows.h>
#include <iostream>
#include <shlobj_core.h>
#include <filesystem>
#include <string>

#pragma comment (lib, "Shell32")

int main()
{
    // Config Path
    std::filesystem::path path;
    PWSTR path_tmp;

    auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_tmp);
    
    if (get_folder_path_ret != S_OK) {
        CoTaskMemFree(path_tmp);
        return 1;
    }
    path = path_tmp;
    CoTaskMemFree(path_tmp);

    path += "\\SOTF\\config\\config.ini";
    std::string path_str = path.string();
    const char* real_path = path_str.c_str();

	ShellExecute(NULL, "open", real_path, NULL, NULL, SW_SHOW);

    // Game Path
    std::filesystem::path path2;
    PWSTR path_tmp2;

    auto get_folder_path_ret2 = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_tmp2);
    
    if (get_folder_path_ret2 != S_OK) {
        CoTaskMemFree(path_tmp2);
        return 1;
    }
    path2 = path_tmp2;
    CoTaskMemFree(path_tmp2);

    path2 += "\\SOTF\\config\\gamePath.ini";
    std::string path_str2 = path2.string();
    const char* real_path2 = path_str2.c_str();

    ShellExecute(NULL, "open", real_path2, NULL, NULL, SW_SHOW);

	return 0;
}