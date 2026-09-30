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
          /____/\_, / /_/|_|\___/\_,_/\_,_/_.__/\___/_.__/\__/_/\_\         \    \/    /
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

bool inject_dll(DWORD ProcessID)
{
    LPCSTR DLL_PATH = R"(C:\Users\Roadb\Documents\Vortex Skidding\CLion Project - win32 api and memory research\UnityEngine.dll)";
    LPVOID LoadLibAddy, RemoteString;

    HANDLE Proc = OpenProcess(CREATE_THREAD_ACCESS, FALSE, ProcessID);

    if (!Proc)
    {
        std::cout << "OpenProcess() failed: " << GetLastError() << std::endl;
        return false;
    }

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_PATH) + 1, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(Proc, RemoteString, (LPVOID)DLL_PATH, strlen(DLL_PATH)+1, NULL);
    CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, RemoteString, NULL, NULL);

    CloseHandle(Proc);

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
    if (dlls.empty()) {
        std::cout << "No dlls found!" << '\n' << std::endl;
    }
    return dlls;
}

void fetch_dll_path()
{

}

int main()
{
    // clear cli and draw banner
    std::system("cls");
    std::cout << banner << "\n\n" << std::endl;

    // if (not IsWindowsXPOrGreater())
    // {
    //     std::cout << "no way????????????" << std::endl;
    //     return 0;
    // }
    // else
    // {
    //     // EnumWindows enumerates and calls our EnumWindowsProc callback function on every top-level window
    //     EnumWindows(EnumWindowsProc , NULL);
    //
    //     std::cout << '\n' << "Please select a process by its PID for injection: ";
    //     DWORD selected_pid;
    //     std::cin >> selected_pid;
    //     std::cout << '\n' << std::endl;
    //     if (!selected_pid)
    //     {
    //         std::cout << "did you actually just press enter without typing anything???" << std::endl;
    //         return 0;
    //     }
    //
        enum_dlls(); // TODO: return
    //
    //     std::cout << '\n' << "Please choose a DLL to be injected by its number: ";
    //     int selected_dll;
    //     std::cin >> selected_dll;
    //     std::cout << '\n' << std::endl;
    //     if (!selected_dll)
    //     {
    //         std::cout << "did you actually just press enter without typing anything???" << std::endl;
    //         return 0;
    //     }

        fetch_dll_path();

        //inject_dll(selected_pid);
    //}
}
