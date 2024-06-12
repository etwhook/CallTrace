#include "./CallTrace/Trace.h"

VOID InitConsole() {
    FILE* conOut;
    AllocConsole();
    SetConsoleTitleA("CallTrace");
    freopen_s(&conOut, "CONOUT$", "w", stdout);

}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        InitConsole();
        SymInitialize(GetCurrentProcess(), NULL, TRUE);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitInstrumentionCallback, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

