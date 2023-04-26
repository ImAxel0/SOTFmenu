#include <Windows.h>
#include <iostream>
#include <shlobj_core.h>
#include <filesystem>

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);

    int result{};
    std::filesystem::path path;
    PWSTR path_tmp;

    auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_tmp);
    // Error check
    if (get_folder_path_ret != S_OK) {
        CoTaskMemFree(path_tmp);
        return 1;
    }
    path = path_tmp;
    CoTaskMemFree(path_tmp);

    std::cout << "Destination path: " << path << "\n\n";

    try
    {
        std::filesystem::copy("Copy", path, std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive);
    }
    catch (const std::filesystem::filesystem_error& e) {
        SetConsoleTextAttribute(hConsole, 4);
        std::cerr << e.what() << std::endl;
        system("pause");
        return 1;
    }
    printf("Copying...\n\n");
    SetConsoleTextAttribute(hConsole, 2);
    std::cout << "SOTF folder copied successfully at: " << path << "\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    system("pause");
    return 0;
}