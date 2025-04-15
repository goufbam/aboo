#pragma once
#include <Windows.h>
#include <tlhelp32.h>
#include "OS-ImGui/OS-ImGui.h"
//自己加驱动         Add your own driver
//驱动加不明白也不要问我，你不能比我还会CV

DWORD GetProcessID(std::string ProcessName)
{
    PROCESSENTRY32 ProcessInfoPE;
    ProcessInfoPE.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(15, 0);
    Process32First(hSnapshot, &ProcessInfoPE);

    do {
        if (strcmp(ProcessInfoPE.szExeFile, ProcessName.c_str()) == 0)
        {
            CloseHandle(hSnapshot);
            return ProcessInfoPE.th32ProcessID;
        }
    } while (Process32Next(hSnapshot, &ProcessInfoPE));
    CloseHandle(hSnapshot);
    return 0;
}




namespace MenuConfig
{
    inline bool ShowBoneESP = true;
    inline bool ShowBoxESP = true;
    inline bool ShowHealthBar = true;
    inline bool ShowWeaponESP = true;
    inline bool ShowDistance = true;
    inline bool ShowEyeRay = true;
    inline bool ShowPlayerName = true;
    inline int TriggerMode = 0;
    inline bool Pressed = false;// for toggle mode
    inline bool Shoot = false;// so that it doesn’t aim when the trigger hits
    inline bool AimBot = true;
    inline int AimBotHotKey = 0;
    // 0: head 1: neck 3: spine
    inline int  AimPosition = 0;
    inline bool ShowAimFovRange = true;
    inline ImColor AimFovRangeColor = ImColor(230, 230, 230, 255);
    inline bool VisibleCheck = true;
    // 0: normal 1: dynamic
    inline int  BoxType = 0;
    // 0: Vertical 1: Horizontal
    inline int  HealthBarType = 0;
    inline bool ShowGlow = true;
    inline ImColor BoneColor = ImColor(255, 51, 51, 255);
    inline ImColor Boneviscolor = ImColor(255, 153, 255, 255);
    inline ImColor BoxColor = ImColor(255, 255, 255, 255);
    inline ImColor EyeRayColor = ImColor(255, 0, 0, 255);

    inline bool ShowMenu = false;


    inline bool ShowHeadShootLine = true;
    inline ImColor HeadShootLineColor = ImColor(255, 255, 255, 255);

    inline bool ShowFovLine = true;
    inline ImColor FovLineColor = ImColor(55, 55, 55, 220);
    inline float FovLineSize = 60.f;

    inline bool ShowLineToEnemy = true;
    inline ImColor LineToEnemyColor = ImColor(255, 255, 255, 220);

    inline bool ShowCrossHair = true;
    inline ImColor CrossHairColor = ImColor(45, 45, 45, 255);
    inline float CrossHairSize = 150;

}