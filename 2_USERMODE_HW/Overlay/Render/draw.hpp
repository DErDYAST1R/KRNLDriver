#define NOMINMAX
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

#include "../../Driver/API/Driver.h"
#include "WinUser.h"

#include <thread>
#include <cmath>

#include "../../Lib/json.hpp"
#include <D3DX11tex.h>

#include <cmath>
#include <psapi.h>
#include <tchar.h>
#include <tchar.h>
#include <thread>

#include "../../Utils/pimraryXor.h"
#include "drawUtils.hpp"
#include <algorithm>
#include <Windows.h>

#include "../../sdk/sdk.h"

namespace config {
    namespace AIMBOT {
        bool EnableAimbot = true;

        bool ShowFov = true;
        bool FillFov = false;

        bool AweomseCrosshair = false;

        bool AimAtTeam = true;

        bool VisibleOnly = true;

        bool ShootingFOVEnabled = true;
        int ShootingFOV = 25;
        int ShootingFOVAfterBullets = 4;

        int StopAfterBullet = 8;

        float Smoothing = 2.5f;
        int FOV = 15;
        float FovCircleColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        float FovFilledCircleColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        bool AimbotKey = true;
        static int selectedKey = 0;
        const char* keys[] = { "MB1", "MB2", "X", "Z", "C", "V", "Left Control", "Left Alt", "Left Shift", "Caps Lock" };
        int virtualKeyCodes[] = { VK_LBUTTON, VK_RBUTTON, 'X', 'Z', 'C', 'V', VK_LCONTROL, VK_LMENU, VK_LSHIFT, VK_CAPITAL };

        bool BetweenShots = false;
        bool OnFirstShot = true;

        bool EnableRecoilReducer = true;

        static int selectedHitbox = 0;
        const char* hitboxes[] = { "Head", "Chest", "Cock" };
        int hitboxCount = IM_ARRAYSIZE(hitboxes);
    }

    namespace ESP {
        bool GlobalEnable = true;
        bool ShowTeam = false;

        bool OnKey = false;
        static int selectedKey = 0;
        const char* keys[] = { "MB1", "MB2", "X", "Z", "C", "V", "Left Control", "Left Alt", "Left Shift", "Caps Lock" };
        int virtualKeyCodes[] = { VK_LBUTTON, VK_RBUTTON, 'X', 'Z', 'C', 'V', VK_LCONTROL, VK_LMENU, VK_LSHIFT, VK_CAPITAL };

        namespace enemy {
            bool EnableESP = true;

            bool EnableBoxEsp = true;
            float BoxColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            bool SkeletonESP = true;
            float SkeletonColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            bool EnableFillBox = false;
            float FillBoxColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

            bool DrawGradiantBox = true;
            float TopColor[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
            float BottomColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };

            bool EnableHeadDot = false;
            float HeadDotColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            float HeadDotFillColor[4] = { 0.7f, 0.0f, 0.0f, 0.5f };

            bool EnableName = true;
            float NameColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        }

        namespace team {
            bool EnableESP = true;

            bool EnableBoxEsp = true;
            float BoxColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            bool SkeletonESP = true;
            float SkeletonColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

            bool EnableFillBox = false;
            float FillBoxColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

            bool DrawGradiantBox = true;
            float TopColor[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
            float BottomColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };

            bool EnableHeadDot = false;
            float HeadDotColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            float HeadDotFillColor[4] = { 0.7f, 0.0f, 0.0f, 0.5f };

            bool EnableName = true;
            float NameColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        }
    }
}

std::vector<Player> PlayerBuffer = {};
void EntityThread() {
    while (true) {
        LocalPlayer LocalObject;
        std::vector<Player> TempBuffer = {};
        for (int i = 1; i < 64; i++) {
            if (LocalObject.LocalPlayerPtr == NULL) {
                PlayerBuffer.clear();
                continue;
            }
            Player PlayerObject(i);
            if (PlayerObject.entity == NULL) continue;
            // Camera CameraObject;
            // PlayerObject.Cache3DBones();
            // PlayerObject.Cache2DBones(CameraObject);

            TempBuffer.push_back(PlayerObject);
        }
        PlayerBuffer = TempBuffer;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void BonesThread() {
    while (TRUE) {
        for (Player& PlayerObject : PlayerBuffer) {
            if (PlayerObject.entity == NULL) continue;
            if (PlayerObject.m_iHealth <= 0) continue;
            Camera CameraObject;

            PlayerObject.Cache3DBones();
            PlayerObject.Cache2DBones(CameraObject);
        }
    }
}

int EntityThreadR = 0;
void HackThread(ImDrawList* drawList) {
    if (EntityThreadR == 0) {
        std::thread b(BonesThread); b.detach();
        std::thread t(EntityThread); t.detach();
        EntityThreadR = 1;
    }
    LocalPlayer LocalObject;
    World World;
    Camera CameraObject;
    // map_loader map;

    for (Player PlayerObject : PlayerBuffer) {
        if (PlayerObject.entity == NULL) continue;
        if (PlayerObject.entity == LocalObject.LocalPlayerPtr) continue;

        if (PlayerObject.m_iHealth <= 0) continue;

        Vector3 absorigin;
        CameraObject.w2s(PlayerObject.origin, absorigin, CameraObject.GetViewMatrix());

        Vector3 bones[] = {
            PlayerObject.Bones2D.Ahead,
            PlayerObject.Bones2D.AshoulderR,
            PlayerObject.Bones2D.AshoulderL,
            PlayerObject.Bones2D.AbrasR,
            PlayerObject.Bones2D.AbrasL,
            PlayerObject.Bones2D.AhandR,
            PlayerObject.Bones2D.AhandL,
            PlayerObject.Bones2D.AkneesR,
            PlayerObject.Bones2D.AkneesL,
            PlayerObject.Bones2D.AfeetR,
            PlayerObject.Bones2D.AfeetL
        };

        Vector3 mostLeftBone;
        Vector3 mostRightBone;
        // Check if bones array is not empty
        if (sizeof(bones) / sizeof(bones[0]) > 0) {
            // Initialize mostLeftBone and mostRightBone with the first bone in the array
            mostLeftBone = bones[0];
            mostRightBone = bones[0];

            // Iterate through the bones array to find the most left and most right bones
            for (const auto& bone : bones) {
                if (bone.x < mostLeftBone.x) mostLeftBone = bone;
                if (bone.x > mostRightBone.x) mostRightBone = bone;
            }

            // Now mostLeftBone and mostRightBone contain the bones with the smallest and largest x values respectively
        }

        if (PlayerObject.Bones2D.Ahead.y > PlayerObject.Bones2D.AfeetL.y)
            continue; // For clamping

        overlay::DrawText2D(drawList, ImVec2(mostLeftBone.x - 5, PlayerObject.Bones2D.Ahead.y - 15), 11.0f, ImColor(255, 255, 255, 255), PlayerObject.name.c_str());

        overlay::DrawBox2D(drawList,
            ImVec2(mostLeftBone.x - 5, PlayerObject.Bones2D.Ahead.y - 5),
            ImVec2(mostRightBone.x + 5, PlayerObject.Bones2D.AfeetR.y + 5),
            ImColor(255, 255, 255, 255));

        // Head to Neck
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.Ahead.x, PlayerObject.Bones2D.Ahead.y),
            ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);

        // Neck to Shoulders
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y),
            ImVec2(PlayerObject.Bones2D.AshoulderR.x, PlayerObject.Bones2D.AshoulderR.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y),
            ImVec2(PlayerObject.Bones2D.AshoulderL.x, PlayerObject.Bones2D.AshoulderL.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);

        // Shoulders to Elbows
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.AshoulderR.x, PlayerObject.Bones2D.AshoulderR.y),
            ImVec2(PlayerObject.Bones2D.AbrasR.x, PlayerObject.Bones2D.AbrasR.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.AshoulderL.x, PlayerObject.Bones2D.AshoulderL.y),
            ImVec2(PlayerObject.Bones2D.AbrasL.x, PlayerObject.Bones2D.AbrasL.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);

        // Elbows to Hands
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.AbrasR.x, PlayerObject.Bones2D.AbrasR.y),
            ImVec2(PlayerObject.Bones2D.AhandR.x, PlayerObject.Bones2D.AhandR.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.AbrasL.x, PlayerObject.Bones2D.AbrasL.y),
            ImVec2(PlayerObject.Bones2D.AhandL.x, PlayerObject.Bones2D.AhandL.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);

        // Neck to Pelvis
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y),
            ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);

        // Pelvis to Knees
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y),
            ImVec2(PlayerObject.Bones2D.AkneesR.x, PlayerObject.Bones2D.AkneesR.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y),
            ImVec2(PlayerObject.Bones2D.AkneesL.x, PlayerObject.Bones2D.AkneesL.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);

        // Knees to Feet
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.AkneesR.x, PlayerObject.Bones2D.AkneesR.y),
            ImVec2(PlayerObject.Bones2D.AfeetR.x, PlayerObject.Bones2D.AfeetR.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);
        overlay::DrawLine2D(drawList,
            ImVec2(PlayerObject.Bones2D.AkneesL.x, PlayerObject.Bones2D.AkneesL.y),
            ImVec2(PlayerObject.Bones2D.AfeetL.x, PlayerObject.Bones2D.AfeetL.y),
            Float4ToImColor(config::ESP::enemy::SkeletonColor),
            1.2f);
    }
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
    style->Colors[ImGuiCol_Separator] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_CheckMark] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_Tab] = ImVec4(0.29f, 0.09f, 0.40f, 0.86f);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(0.21f, 0.09f, 0.27f, 0.80f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.49f, 0.00f, 0.54f, 1.00f);
    style->TabRounding = 0.0f;
    style->WindowRounding = 5.0f;
    style->FrameRounding = 4.0f;

    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;
}

bool LoadedStyles = FALSE;
bool ShowMenu = FALSE;
bool Buys = FALSE;
void DrawMenu() {
    SPOOF_FUNC;

    if (LoadedStyles == FALSE) {
        RunStyle();
        LoadedStyles = TRUE;
    }
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        ShowMenu = !ShowMenu;
    }

    if (ShowMenu) {
        ImGui::SetNextWindowSize({ 400, 450 });
        if (ImGui::Begin(xorstr_("SelfLeak"), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {

            ImGui::Checkbox("Cum", &Buys);
        }
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

    if (Driver->BaseAddress != NULL) {
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

        DrawOverlay();
        DrawMenu();
    }
}