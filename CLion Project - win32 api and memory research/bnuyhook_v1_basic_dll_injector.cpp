#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <Windows.h>
#include <VersionHelpers.h>

std::string banner = R"(   _______   ,---.   .--.  ___    _    ____     __ .---.  .---.     ,-----.        ,-----.    .--.   .--.
  \  ____  \ |    \  |  |.'   |  | |   \   \   /  /|   |  |_ _|   .'  .-,  '.    .'  .-,  '.  |  | _/  /
  | |    \ | |  ,  \ |  ||   .'  | |    \  _. /  ' |   |  ( ' )  / ,-.|  \ _ \  / ,-.|  \ _ \ | (`' ) /
  | |____/ / |  |\_ \|  |.'  '_  | |     _( )_ .'  |   '-(_{;}_);  \  '_ /  | :;  \  '_ /  | :|(_ ()_)
  |   _ _ '. |  _( )_\  |'   ( \.-.| ___(_ o _)'   |      (_,_) |  _`,/ \ _/  ||  _`,/ \ _/  || (_,_)   __
  |  ( ' )  \| (_ o _)  |' (`. _` /||   |(_,_)'    | _ _--.   | : (  '\_/ \   ;: (  '\_/ \   ;|  |\ \  |  |
  | (_{;}_) ||  (_,_)\  || (_ (_) _)|   `-'  /     |( ' ) |   |  \ `"/  \  ) /  \ `"/  \  ) / |  | \ `'   /
  |  (_,_)  /|  |    |  | \ /  . \ / \      /      (_{;}_)|   |   '. \_/``".'    '. \_/``".'  |  |  \    /
  /_______.' '--'    '--'  ``-'`-''   `-..-'       '(_,_) '---'     '-----'        '-----'    `--'   `'-'
             ___         ___               ____        __       __         ___        ___
            / _ )__ __  / _ \___  ___ ____/ / /  ___  / /  ___ / /__     /     \    /     \
           / _  / // / / , _/ _ \/ _ `/ _  / _ \/ _ \/ _ \/ -_)  '_/      \     \  /     /
          /____/\_, / /_/|_|\___/\_,_/\_,_/_.__/\___/_.__/\__/_/\_\         \    \/    /        v1.0.0
               /___/)";

BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
{
    int length = GetWindowTextLength(hwnd);
    char* buffer = new char[length + 1]; // TODO: replace with modern cpp 20/23 features
    GetWindowText(hwnd, buffer, length + 1);
    std::string windowTitle(buffer);
    delete[] buffer;
    DWORD dwProcessId;
    // DWORD dwThreadId, dwProcessId;

    // list visible windows with a non-empty title
    if (IsWindowVisible(hwnd) && length != 0) {
        GetWindowThreadProcessId(hwnd, &dwProcessId);
        // dwThreadId = GetWindowThreadProcessId(hwnd, &dwProcessId);
        std::cout << "       > PID - " << dwProcessId << ":  " << windowTitle << std::endl;
    }
    return TRUE;
}

#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)

bool inject_dll(DWORD ProcessID, const std::filesystem::path &dll)
{
    std::filesystem::path dll_dir = std::filesystem::current_path() / dll;
    std::string dll_dir_str = dll_dir.string();
    LPCSTR DLL_PATH = dll_dir_str.c_str();
    //std::cout << "DLL PATH: " << DLL_PATH << std::endl;
    // LPCSTR DLL_PATH = R"(C:\Users\Roadb\Documents\Vortex Skidding\CLion Project - win32 api and memory research\UnityEngine.dll)";
    LPVOID LoadLibAddy, RemoteString;

    HANDLE Proc = OpenProcess(CREATE_THREAD_ACCESS, FALSE, ProcessID);

    if (!Proc)
    {
        std::cout << "OpenProcess() failed: " << GetLastError() << std::endl;
        return false;
    }

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    // TODO: why Null not nullptr?
    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_PATH) + 1, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(Proc, RemoteString, (LPVOID)DLL_PATH, strlen(DLL_PATH)+1, NULL);
    CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, RemoteString, NULL, NULL);

    CloseHandle(Proc);

    // TODO: print process name and dll name?
    std::cout << "DLL Injected!" << '\n' << std::endl;

    return true;
}

std::vector<std::filesystem::path> enum_dlls()
{
    int dll_count = 0;
    std::vector<std::filesystem::path> dlls = {};
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        // filter to ensure only .dll files are listed and exclude subdirectories
        if (std::filesystem::is_regular_file(entry.status()) and std::filesystem::path(entry).extension() == ".dll") {
            dll_count++;
            dlls.emplace_back(entry.path());
            std::cout << dll_count << ": " << entry.path().filename().string() << std::endl;
        }
    }
    return dlls;
}

std::filesystem::path fetch_dll_path(const std::vector<std::filesystem::path> &dlls, const int &selection)
{
    return dlls[selection - 1];
}

int main()
{
    // clear cli and draw banner
    std::system("cls");
    std::cout << banner << "\n\n" << std::endl;

    if (not IsWindowsXPOrGreater())
    {
        std::cout << "no way????????????" << std::endl;
        return 0;
    }
    else
    {
        // EnumWindows enumerates and calls our EnumWindowsProc callback function on every top-level window
        EnumWindows(EnumWindowsProc , NULL);

        std::cout << '\n' << "Please select a process by its PID for injection: ";
        DWORD selected_pid;
        std::cin >> selected_pid;
        std::cout << '\n' << std::endl;
        if (!selected_pid)
        {
            std::cout << "did you actually just type 0 or press enter without typing anything???" << std::endl;
            return 0;
        }

        const std::vector<std::filesystem::path> dlls = enum_dlls();

        if (dlls.empty())
        {
            std::cout << "No dlls found!" << '\n' << std::endl;
            return 0;
        }

        std::cout << '\n' << "Please choose a DLL to be injected by its number: ";
        int dll_selection;
        std::cin >> dll_selection;
        std::cout << '\n';
        //if (!dll_selection)
        // {
        //     std::cout << "did you actually just press enter without typing anything???" << std::endl;
        //     return 0;
        // }
        if (dll_selection <= 0 or dll_selection > dlls.size())
        {
            std::cout << "not in range bud, there are " << dlls.size() << " dlls..." << std::endl;
            return 0;
        }

        std::filesystem::path final_dll = fetch_dll_path(dlls, dll_selection);

        inject_dll(selected_pid, final_dll);
    }
}