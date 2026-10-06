#include <windows.h>

// BOOL = return value used by windows to know if the dll was loaded / the DllMain function executed successfully.
// APIENTRY = calling convention specifier, 32-bit leftover.
// DllMain(
// HMODULE hinstDLL, = handle to the loaded module, base address of the PE image in memory, IMAGE_DOS_HEADER, address of DLL in memory.
// DWORD fdwReason = reason for calling the function, (DLL_PROCESS_ATTACH, DLL_PROCESS_DETACH, DLL_THREAD_ATTACH, DLL_THREAD_DETACH).
// LPVOID lpvReserved = reserved, (serves as a boolean indicator to distinguish between static (implicit) and dynamic (explicit) DLL loading or unloading operations). ? ? ? ?
// )

BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD actionReason, LPVOID reservedPointer) {
    switch (actionReason) {
    case DLL_PROCESS_ATTACH:
        MessageBoxA(nullptr, "(  \\_/  )  Hello from BNUYHOOK!\n(> . o)  The DLL was loaded into the process.", "BNUYHOOK - DLL_PROCESS_ATTACH", MB_OK | MB_ICONWARNING); // yo whats up
        break;
    case DLL_PROCESS_DETACH:
        MessageBoxA(nullptr, "(  \\_/  )  Goodbye from BNUYHOOK...\n(> . o)  The DLL is being unloaded from the process.", "BNUYHOOK - DLL_PROCESS_DETACH", MB_OK | MB_ICONWARNING);
        break;
    }
    return TRUE; // dll successfully loaded, #define TRUE 1.
}