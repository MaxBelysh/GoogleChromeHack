// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "ChromeThread.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(NULL, 0, CreateChromeThread, NULL, 0, NULL);

        if (!hThread) {
            MessageBoxA(NULL, "Failed to create Thread", NULL, MB_OK);
        }
        else {
            CloseHandle(hThread);
        }

        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

