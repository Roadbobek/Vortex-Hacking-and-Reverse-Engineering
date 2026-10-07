#include <windows.h>

// BOOL = return value used by windows to know if the DLL was loaded / the DllMain function executed successfully, typedef for an int 0 / 1.
// APIENTRY = calling convention specifier, 32-bit leftover.
// DllMain(
// HMODULE hinstDLL, = handle to the loaded module, base address of the PE image in memory, IMAGE_DOS_HEADER, address of DLL in memory.
// DWORD fdwReason = reason for calling the function, (DLL_PROCESS_ATTACH, DLL_PROCESS_DETACH, DLL_THREAD_ATTACH, DLL_THREAD_DETACH, (1, 0, 2, 3)).
// LPVOID lpvReserved = serves as a boolean indicator to distinguish between static (implicit) and dynamic (explicit) DLL loading or unloading operations, (NULL / non-NULL).
// ) ( More in-depth notes in notes.txt, in parent directory )

BOOL APIENTRY DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        MessageBoxA(nullptr, "(  \\_/  )  Hello from BNUYHOOK!\n(> . o)  The DLL was loaded into the process.", "BNUYHOOK - DLL_PROCESS_ATTACH", MB_OK | MB_ICONWARNING); // yo whats up
        DisableThreadLibraryCalls(hinstDLL); // disables running DllMain on every thread attachment and detachment in target process.
        break;
    case DLL_PROCESS_DETACH:
        MessageBoxA(nullptr, "(  \\_/  )  Goodbye from BNUYHOOK...\n(> . o)  The DLL is being unloaded from the process.", "BNUYHOOK - DLL_PROCESS_DETACH", MB_OK | MB_ICONWARNING);
        break;
    }
    return TRUE; // dll successfully loaded, #define TRUE 1.
}