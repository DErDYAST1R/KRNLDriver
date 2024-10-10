#define NOMINMAX
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

#include "../Driver/API/Driver.h"

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

void hackthread(ImDrawList* drawList) {
    Driver->BaseAddress = Driver->FindBaseAddress();

}

void DrawOverlay() {
    ImGui::Begin(xorstr_("renderer"), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs );
    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetWindowSize(ImVec2(1920, 1080)); // W , H
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    /* Overlay */
    hackthread(drawList);

    ImGui::End();
}

ID3D11ShaderResourceView* Image = nullptr;

bool ShowMenu = true;
bool StyleLoaded = false;

void RunStyle() {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.0f, 0.0263f, 0.0357f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_ChildBg] = ImColor(15, 15, 15);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.080f, 0.080f, 0.080f, 0.940f);
    style->Colors[ImGuiCol_Border] = ImColor(57, 57, 57);
    style->Colors[ImGuiCol_BorderShadow] = ImColor(1, 1, 1);
    style->Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40);
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(40, 40, 40);
    style->Colors[ImGuiCol_FrameBgActive] = ImColor(40, 40, 40);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.263f, 0.357f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    //style->Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_SliderGrab] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_Button] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_ButtonActive] = ImColor(20, 20, 20);
    style->Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    //style->Colors[ImGuiCol_Column] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    //style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
//	style->Colors[ImGuiCol] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    //style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    style->Colors[ImGuiCol_Separator] = ImColor(255,182,193);
    style->Colors[ImGuiCol_CheckMark] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_Tab] = ImVec4(0.29f, 0.09f, 0.40f, 0.86f);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(0.21f, 0.09f, 0.27f, 0.80f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.49f, 0.00f, 0.54f, 1.00f);
    style->TabRounding = 0.0f;
    style->WindowRounding = 5.0f;
    style->FrameRounding = 4.0f;

    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;

    StyleLoaded = !StyleLoaded;
}


int tabb = 1;
void DrawMenu() {
    /* Overlay Shit */
    if (!StyleLoaded)
        RunStyle();
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        ShowMenu = !ShowMenu;
    }

    HWND window = FindWindowW(NULL, xorstr_(L"U"));
    LONG_PTR exStyle = GetWindowLongPtr(window, GWL_EXSTYLE);
    if (ShowMenu)
    {
        ImGuiIO& io = ImGui::GetIO();

        exStyle &= ~WS_EX_TRANSPARENT;

        bool show_demo_window = false;
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        /* BG BEHIND */
        // Begin the ImGui window
        ImGuiStyle* style = &ImGui::GetStyle();

        /* Menu */
        ImGui::Begin(xorstr_("i64NtNvda"), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
        style = &ImGui::GetStyle();
        style->Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        ImVec2 MainWindowPos = ImGui::GetWindowPos();
        // MENU LOGIC

        ImGui::End();
    }
    else {
        exStyle |= WS_EX_TRANSPARENT;
    }
    SetWindowLongPtr(window, GWL_EXSTYLE, exStyle);
    SetWindowPos(window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    ImGui::End();
}


bool IsProcessForegroundWindow(DWORD processID) {
    HWND foregroundWindow = GetForegroundWindow();
    if (foregroundWindow == NULL) {
        return false;
    }

    DWORD foregroundProcessID;
    GetWindowThreadProcessId(foregroundWindow, &foregroundProcessID);

    return processID == foregroundProcessID;
}

void drawLoop() {
    if (GetAsyncKeyState(VK_END))
        exit(1);

    if (IsProcessForegroundWindow(Driver->ProcessID) || IsProcessForegroundWindow(_getpid())) {
        DrawOverlay();
        DrawMenu();
    }

}