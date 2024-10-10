#define NOMINMAX
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

#include "../Driver/API/Driver.h"
#include "WinUser.h"

#include <thread>
#include <cmath>

#include "../Lib/json.hpp"
#include <D3DX11tex.h>

#include <cmath>
#include <psapi.h>
#include <tchar.h>
#include <tchar.h>
#include <thread>

#include "../Utils/pimraryXor.h"
#include "drawUtils.hpp"
#include <algorithm>
#include <Windows.h>

#include "../sdk/sdk.h"

void HackThread(ImDrawList* drawList) {

}

void DrawOverlay() {
    SPOOF_FUNC;
    ImGui::Begin(xorstr_("renderer"), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs);
    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetWindowSize(ImVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))); // W , H
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    /* Overlay */
    HackThread(drawList);

    ImGui::End();
}

void DrawMenu() {
    SPOOF_FUNC;
    ImGui::SetNextWindowSize({ 400, 450 });
    if (ImGui::Begin(xorstr_("SelfLeak"), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
    {
        bool SexBool = FALSE;
        ImGui::Checkbox("Click for Sex!", &SexBool);
    }
    ImGui::End();
}

bool IsProcessForegroundWindow(DWORD processID) {
    SPOOF_FUNC;
    HWND foregroundWindow = GetForegroundWindow();
    if (foregroundWindow == NULL) {
        return false;
    }

    DWORD foregroundProcessID;
    GetWindowThreadProcessId(foregroundWindow, &foregroundProcessID);

    return processID == foregroundProcessID;
}

void drawLoop() {
    SPOOF_FUNC;
    ImGuiIO& io = ImGui::GetIO();
    POINT p;
    GetCursorPos(&p);
    io.MousePos.x = p.x;
    io.MousePos.y = p.y;

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        io.MouseDown[0] = true;
        io.MouseClicked[0] = true;
        io.MouseClickedPos[0].x = io.MousePos.x;
        io.MouseClickedPos[0].x = io.MousePos.y;
    }
    else {
        io.MouseDown[0] = false;
    }

    if (IsProcessForegroundWindow(Driver->ProcessID) || IsProcessForegroundWindow(_getpid())) {
        DrawOverlay();
        DrawMenu();
    }

}