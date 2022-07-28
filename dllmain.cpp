// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include "csgotrace.h"
#include "csgosdk.h"
#include "csgo.hpp"

tTraceRay TraceRay;
tCreateInterface CreateInterface;

DWORD WINAPI hackThead(HMODULE hModule) 
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    uintptr_t clientdll = (uintptr_t)GetModuleHandle("client.dll");
    uintptr_t enginedll = (uintptr_t)GetModuleHandle("engine.dll");

    CreateInterface = (tCreateInterface)GetProcAddress((HMODULE)enginedll, "CreateInterface");
    IEngineTrace* EngineTrace = (IEngineTrace*)GetInterface(CreateInterface, "EngineTraceClient004");

    while (!GetAsyncKeyState(VK_END))
    {
        Ent* pLocal = *(Ent**)(clientdll + offsets::signatures::dwLocalPlayer);
        EntList* entList = (EntList*)(clientdll + offsets::signatures::dwEntityList);

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            for (auto currEnt : entList->entListObjs)
            {
                if (currEnt.ent && currEnt.ent->clientId != pLocal->clientId)
                {
                    Vector eyepos = pLocal->origin + pLocal->m_vecViewOffset;
                    Vector targeteyepos = currEnt.ent->origin + currEnt.ent->m_vecViewOffset;

                    CGameTrace trace;
                    Ray_t ray;
                    CTraceFilter tracefilter;
                    tracefilter.pSkip = (void*)pLocal;

                    ray.Init(eyepos, targeteyepos);

                    EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &tracefilter, &trace);

                    if (currEnt.ent == trace.m_pEnt)
                    {
                        std::cout << "Ent 0x" << std::hex << currEnt.ent << "is visible\n";
                    }
                }
            }
        }
        Sleep(50);
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hThead = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hackThead, hModule, 0, 0);
        if (hThead) CloseHandle(hThead);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

