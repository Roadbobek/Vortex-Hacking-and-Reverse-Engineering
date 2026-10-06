#include <windows.h>
// not needed since we link the user32 library in CMakeLists, also this only works for the msvc compiler, default for vs toolchain.
//#pragma comment (lib, "user32.lib")

// TODO: calling stuff, especially MessageBoxA is dangerous inside DllMain, make a new thread!
BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD actionReason, LPVOID reservedPointer) {
    switch (actionReason) {
    // runs only when our dll is loaded into a process
    case DLL_PROCESS_ATTACH:
        MessageBoxA(nullptr, "(  \\_/  )  Hello from BNUYHOOK!\n(> . o)  The DLL was loaded into the process.", "BNUYHOOK - DLL_PROCESS_ATTACH", MB_OK | MB_ICONWARNING);
        break;
    // runs only when our dll is unloaded from a process
    case DLL_PROCESS_DETACH:
        MessageBoxA(nullptr, "(  \\_/  )  Goodbye from BNUYHOOK...\n(> . o)  The DLL is being unloaded from the process.", "BNUYHOOK - DLL_PROCESS_DETACH", MB_OK | MB_ICONWARNING);
        break;
    // this runs for every and any thread that is attached in the process, it will flood like crazy
    //case DLL_THREAD_ATTACH:
        MessageBoxA(nullptr, "(  \\_/  )  A BNUYHOOK thread was created.\n(> . o)  The DLL received a thread attach notification.", "BNUYHOOK - DLL_THREAD_ATTACH", MB_OK | MB_ICONWARNING);
        break;
    // this runs for every and any thread that is detached in the process, it will flood like crazy
    //case DLL_THREAD_DETACH:
        MessageBoxA(nullptr, "(  \\_/  )  A BNUYHOOK thread is exiting.\n(> . o)  The DLL received a thread detach notification.", "BNUYHOOK - DLL_THREAD_DETACH", MB_OK | MB_ICONWARNING);
        break;
    }
    return TRUE; // dll successfully loaded
}
