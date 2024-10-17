#pragma once
#include <iostream>
#include <fstream>
#include "../../../Lib/json.hpp"
#include <Windows.h>

using json = nlohmann::json;

namespace CONFIG {
    namespace AIMBOT {
        bool EnableRecoilReducer = true;
        int RCSamt = 3;

        bool EnableAimbot = false;
        bool ShowFov = false;
        bool FillFov = false;
        bool AimAtTeam = false;
        bool VisibleOnly = false;


        bool ShootSettings = false;
        int ShootingFOV = 25;
        float ShootingSpeedX = 2.75f;
        float ShootingSpeedY = 2.75f;
        int ShootingFOVAfterBullets = 4;
        int StopAfterBullet = 8;


        int FOV = 15;
        float SpeedX = 2.4;
        float SpeedY = 2.4;

        float FovCircleColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        float FovFilledCircleColor[4] = { 1.0f, 1.0f, 1.0f, 0.4f };

        bool AimbotKey = false;
        static int selectedKey = 0;
        const char* keys[] = { "MB1", "MB2", "X", "Z", "C", "V", "Left Control", "Left Alt", "Left Shift", "Caps Lock" };
        int virtualKeyCodes[] = { VK_LBUTTON, VK_RBUTTON, 'X', 'Z', 'C', 'V', VK_LCONTROL, VK_LMENU, VK_LSHIFT, VK_CAPITAL };

        bool OnFirstShot = false;
        static int selectedHitbox = 0;
        static int selectedPrio = 0;
        const char* hitboxes[] = { "Progression", "Nearest", "Head", "Neck", "Chest" };
        const char* prio[] = { "None", "Head", "Neck", "Chest" };
    }

    namespace ESP {
        bool EnableBoxESP = FALSE;
        bool VisibleOnly = FALSE;
        float RoundedBox = 0.3;

        bool on_key = FALSE;
        static int selectedKey = 0;
        const char* keys[] = { "MB1", "MB2", "X", "Z", "C", "V", "Left Control", "Left Alt", "Left Shift", "Caps Lock" };
        int virtualKeyCodes[] = { VK_LBUTTON, VK_RBUTTON, 'X', 'Z', 'C', 'V', VK_LCONTROL, VK_LMENU, VK_LSHIFT, VK_CAPITAL };

        bool EanbleHealthbar = FALSE;
        bool EnableSkeletonESP = FALSE;
        float SkeletonThickness = 1.8f;
        bool SkeletonOutline = FALSE;

        bool EnableNameESP = FALSE;
        bool SnapLine = FALSE;

        bool Fillbox = FALSE;
        bool GradiantBox = FALSE;

        int ExpandF = 4;
        int ThicknessOutline = 1;
        static int BoxSelected = 0;
        const char* boxes[] = { "Outline", "Fill", "Gradiant" };


        float HealthFull[4] = { 0.0f, 0.6f, 0.0f, 0.6f };
        float HealthEmpty[4] = { 0.6f, 0.0f, 0.0f, 0.6f };
        namespace Visible {
            float Outline[4] = { 0.7f, 0.0f, 0.0f, 0.7f };
            float Skeleton[4] = { 0.7f, 0.0f, 0.0f, 0.7f };

            float Name[4] = { 0.7f, 0.0f, 0.0f, 0.7f };
            float SnapLine[4] = { 0.7f, 0.0f, 0.0f, 0.7f };

            float FillColor1[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            float FillColor2[4] = { 0.7f, 0.0f, 0.0f, 0.7f };
        }

        namespace InVisible {
            float Outline[4] = { 0.0f, 0.7f, 0.0f, 0.7f };
            float Skeleton[4] = { 0.0f, 0.7f, 0.0f, 0.7f };

            float Name[4] = { 0.0f, 7.0f, 0.0f, 0.7f };
            float SnapLine[4] = { 0.0f, 7.0f, 0.0f, 0.7f };

            float FillColor1[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            float FillColor2[4] = { 0.0f, 0.7f, 0.0f, 0.7f };
        }
    }
}


void saveConfig() {
    nlohmann::json j;

    // Save AIMBOT settings
    j[xorstr_("AIMBOT")][xorstr_("EnableRecoilReducer")] = CONFIG::AIMBOT::EnableRecoilReducer;
    j[xorstr_("AIMBOT")][xorstr_("RCSamt")] = CONFIG::AIMBOT::RCSamt;
    j[xorstr_("AIMBOT")][xorstr_("EnableAimbot")] = CONFIG::AIMBOT::EnableAimbot;
    j[xorstr_("AIMBOT")][xorstr_("ShowFov")] = CONFIG::AIMBOT::ShowFov;
    j[xorstr_("AIMBOT")][xorstr_("FillFov")] = CONFIG::AIMBOT::FillFov;
    j[xorstr_("AIMBOT")][xorstr_("AimAtTeam")] = CONFIG::AIMBOT::AimAtTeam;
    j[xorstr_("AIMBOT")][xorstr_("VisibleOnly")] = CONFIG::AIMBOT::VisibleOnly;
    j[xorstr_("AIMBOT")][xorstr_("ShootSettings")] = CONFIG::AIMBOT::ShootSettings;
    j[xorstr_("AIMBOT")][xorstr_("ShootingFOV")] = CONFIG::AIMBOT::ShootingFOV;
    j[xorstr_("AIMBOT")][xorstr_("ShootingSpeedX")] = CONFIG::AIMBOT::ShootingSpeedX;
    j[xorstr_("AIMBOT")][xorstr_("ShootingSpeedY")] = CONFIG::AIMBOT::ShootingSpeedY;
    j[xorstr_("AIMBOT")][xorstr_("ShootingFOVAfterBullets")] = CONFIG::AIMBOT::ShootingFOVAfterBullets;
    j[xorstr_("AIMBOT")][xorstr_("StopAfterBullet")] = CONFIG::AIMBOT::StopAfterBullet;
    j[xorstr_("AIMBOT")][xorstr_("FOV")] = CONFIG::AIMBOT::FOV;
    j[xorstr_("AIMBOT")][xorstr_("SpeedX")] = CONFIG::AIMBOT::SpeedX;
    j[xorstr_("AIMBOT")][xorstr_("SpeedY")] = CONFIG::AIMBOT::SpeedY;
    j[xorstr_("AIMBOT")][xorstr_("FovCircleColor")] = { CONFIG::AIMBOT::FovCircleColor[0], CONFIG::AIMBOT::FovCircleColor[1],
                                       CONFIG::AIMBOT::FovCircleColor[2], CONFIG::AIMBOT::FovCircleColor[3] };
    j[xorstr_("AIMBOT")][xorstr_("FovFilledCircleColor")] = { CONFIG::AIMBOT::FovFilledCircleColor[0], CONFIG::AIMBOT::FovFilledCircleColor[1],
                                             CONFIG::AIMBOT::FovFilledCircleColor[2], CONFIG::AIMBOT::FovFilledCircleColor[3] };
    j[xorstr_("AIMBOT")][xorstr_("AimbotKey")] = CONFIG::AIMBOT::AimbotKey;
    j[xorstr_("AIMBOT")][xorstr_("selectedKey")] = CONFIG::AIMBOT::selectedKey;
    j[xorstr_("AIMBOT")][xorstr_("OnFirstShot")] = CONFIG::AIMBOT::OnFirstShot;
    j[xorstr_("AIMBOT")][xorstr_("selectedHitbox")] = CONFIG::AIMBOT::selectedHitbox;
    j[xorstr_("AIMBOT")][xorstr_("selectedPrio")] = CONFIG::AIMBOT::selectedPrio;

    // Save ESP settings
    j[xorstr_("ESP")][xorstr_("EnableBoxESP")] = CONFIG::ESP::EnableBoxESP;
    j[xorstr_("ESP")][xorstr_("VisibleOnly")] = CONFIG::ESP::VisibleOnly;
    j[xorstr_("ESP")][xorstr_("RoundedBox")] = CONFIG::ESP::RoundedBox;
    j[xorstr_("ESP")][xorstr_("on_key")] = CONFIG::ESP::on_key;
    j[xorstr_("ESP")][xorstr_("selectedKey")] = CONFIG::ESP::selectedKey;
    j[xorstr_("ESP")][xorstr_("EnableHealthbar")] = CONFIG::ESP::EanbleHealthbar;
    j[xorstr_("ESP")][xorstr_("EnableSkeletonESP")] = CONFIG::ESP::EnableSkeletonESP;
    j[xorstr_("ESP")][xorstr_("SkeletonThickness")] = CONFIG::ESP::SkeletonThickness;
    j[xorstr_("ESP")][xorstr_("SkeletonOutline")] = CONFIG::ESP::SkeletonOutline;
    j[xorstr_("ESP")][xorstr_("EnableNameESP")] = CONFIG::ESP::EnableNameESP;
    j[xorstr_("ESP")][xorstr_("SnapLine")] = CONFIG::ESP::SnapLine;
    j[xorstr_("ESP")][xorstr_("Fillbox")] = CONFIG::ESP::Fillbox;
    j[xorstr_("ESP")][xorstr_("GradiantBox")] = CONFIG::ESP::GradiantBox;
    j[xorstr_("ESP")][xorstr_("ExpandF")] = CONFIG::ESP::ExpandF;
    j[xorstr_("ESP")][xorstr_("ThicknessOutline")] = CONFIG::ESP::ThicknessOutline;
    j[xorstr_("ESP")][xorstr_("BoxSelected")] = CONFIG::ESP::BoxSelected;
    j[xorstr_("ESP")][xorstr_("HealthFull")] = { CONFIG::ESP::HealthFull[0], CONFIG::ESP::HealthFull[1],
                                CONFIG::ESP::HealthFull[2], CONFIG::ESP::HealthFull[3] };
    j[xorstr_("ESP")][xorstr_("HealthEmpty")] = { CONFIG::ESP::HealthEmpty[0], CONFIG::ESP::HealthEmpty[1],
                                CONFIG::ESP::HealthEmpty[2], CONFIG::ESP::HealthEmpty[3] };

    // Save Visible settings
    j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("Outline")] = { CONFIG::ESP::Visible::Outline[0], CONFIG::ESP::Visible::Outline[1],
                                        CONFIG::ESP::Visible::Outline[2], CONFIG::ESP::Visible::Outline[3] };
    j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("Skeleton")] = { CONFIG::ESP::Visible::Skeleton[0], CONFIG::ESP::Visible::Skeleton[1],
                                         CONFIG::ESP::Visible::Skeleton[2], CONFIG::ESP::Visible::Skeleton[3] };
    j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("Name")] = { CONFIG::ESP::Visible::Name[0], CONFIG::ESP::Visible::Name[1],
                                     CONFIG::ESP::Visible::Name[2], CONFIG::ESP::Visible::Name[3] };
    j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("SnapLine")] = { CONFIG::ESP::Visible::SnapLine[0], CONFIG::ESP::Visible::SnapLine[1],
                                         CONFIG::ESP::Visible::SnapLine[2], CONFIG::ESP::Visible::SnapLine[3] };
    j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("FillColor1")] = { CONFIG::ESP::Visible::FillColor1[0], CONFIG::ESP::Visible::FillColor1[1],
                                            CONFIG::ESP::Visible::FillColor1[2], CONFIG::ESP::Visible::FillColor1[3] };
    j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("FillColor2")] = { CONFIG::ESP::Visible::FillColor2[0], CONFIG::ESP::Visible::FillColor2[1],
                                            CONFIG::ESP::Visible::FillColor2[2], CONFIG::ESP::Visible::FillColor2[3] };

    // Save InVisible settings
    j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("Outline")] = { CONFIG::ESP::InVisible::Outline[0], CONFIG::ESP::InVisible::Outline[1],
                                          CONFIG::ESP::InVisible::Outline[2], CONFIG::ESP::InVisible::Outline[3] };
    j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("Skeleton")] = { CONFIG::ESP::InVisible::Skeleton[0], CONFIG::ESP::InVisible::Skeleton[1],
                                           CONFIG::ESP::InVisible::Skeleton[2], CONFIG::ESP::InVisible::Skeleton[3] };
    j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("Name")] = { CONFIG::ESP::InVisible::Name[0], CONFIG::ESP::InVisible::Name[1],
                                       CONFIG::ESP::InVisible::Name[2], CONFIG::ESP::InVisible::Name[3] };
    j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("SnapLine")] = { CONFIG::ESP::InVisible::SnapLine[0], CONFIG::ESP::InVisible::SnapLine[1],
                                            CONFIG::ESP::InVisible::SnapLine[2], CONFIG::ESP::InVisible::SnapLine[3] };
    j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("FillColor1")] = { CONFIG::ESP::InVisible::FillColor1[0], CONFIG::ESP::InVisible::FillColor1[1],
                                              CONFIG::ESP::InVisible::FillColor1[2], CONFIG::ESP::InVisible::FillColor1[3] };
    j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("FillColor2")] = { CONFIG::ESP::InVisible::FillColor2[0], CONFIG::ESP::InVisible::FillColor2[1],
                                              CONFIG::ESP::InVisible::FillColor2[2], CONFIG::ESP::InVisible::FillColor2[3] };

    // Serialize and save to file
    std::ofstream outFile(xorstr_("C:\\config.json"));
    outFile << j.dump(4); // pretty print with 4 spaces
    outFile.close();
}

void loadConfig() {
    nlohmann::json j;

    std::ifstream inFile(xorstr_("C:\\config.json"));
    if (!inFile.is_open()) {
        std::cerr << xorstr_("Could not open config file!") << std::endl;
        return;
    }

    inFile >> j;

    // Load AIMBOT settings
    CONFIG::AIMBOT::EnableRecoilReducer = j[xorstr_("AIMBOT")][xorstr_("EnableRecoilReducer")].get<bool>();
    CONFIG::AIMBOT::RCSamt = j[xorstr_("AIMBOT")][xorstr_("RCSamt")].get<int>();
    CONFIG::AIMBOT::EnableAimbot = j[xorstr_("AIMBOT")][xorstr_("EnableAimbot")].get<bool>();
    CONFIG::AIMBOT::ShowFov = j[xorstr_("AIMBOT")][xorstr_("ShowFov")].get<bool>();
    CONFIG::AIMBOT::FillFov = j[xorstr_("AIMBOT")][xorstr_("FillFov")].get<bool>();
    CONFIG::AIMBOT::AimAtTeam = j[xorstr_("AIMBOT")][xorstr_("AimAtTeam")].get<bool>();
    CONFIG::AIMBOT::VisibleOnly = j[xorstr_("AIMBOT")][xorstr_("VisibleOnly")].get<bool>();
    CONFIG::AIMBOT::ShootSettings = j[xorstr_("AIMBOT")][xorstr_("ShootSettings")].get<bool>();
    CONFIG::AIMBOT::ShootingFOV = j[xorstr_("AIMBOT")][xorstr_("ShootingFOV")].get<int>();
    CONFIG::AIMBOT::ShootingSpeedX = j[xorstr_("AIMBOT")][xorstr_("ShootingSpeedX")].get<float>();
    CONFIG::AIMBOT::ShootingSpeedY = j[xorstr_("AIMBOT")][xorstr_("ShootingSpeedY")].get<float>();
    CONFIG::AIMBOT::ShootingFOVAfterBullets = j[xorstr_("AIMBOT")][xorstr_("ShootingFOVAfterBullets")].get<int>();
    CONFIG::AIMBOT::StopAfterBullet = j[xorstr_("AIMBOT")][xorstr_("StopAfterBullet")].get<int>();
    CONFIG::AIMBOT::FOV = j[xorstr_("AIMBOT")][xorstr_("FOV")].get<int>();
    CONFIG::AIMBOT::SpeedX = j[xorstr_("AIMBOT")][xorstr_("SpeedX")].get<float>();
    CONFIG::AIMBOT::SpeedY = j[xorstr_("AIMBOT")][xorstr_("SpeedY")].get<float>();

    for (size_t i = 0; i < 4; ++i) {
        CONFIG::AIMBOT::FovCircleColor[i] = j[xorstr_("AIMBOT")][xorstr_("FovCircleColor")][i].get<float>();
        CONFIG::AIMBOT::FovFilledCircleColor[i] = j[xorstr_("AIMBOT")][xorstr_("FovFilledCircleColor")][i].get<float>();
    }

    CONFIG::AIMBOT::AimbotKey = j[xorstr_("AIMBOT")][xorstr_("AimbotKey")].get<bool>();
    CONFIG::AIMBOT::selectedKey = j[xorstr_("AIMBOT")][xorstr_("selectedKey")].get<int>();
    CONFIG::AIMBOT::OnFirstShot = j[xorstr_("AIMBOT")][xorstr_("OnFirstShot")].get<bool>();
    CONFIG::AIMBOT::selectedHitbox = j[xorstr_("AIMBOT")][xorstr_("selectedHitbox")].get<int>();
    CONFIG::AIMBOT::selectedPrio = j[xorstr_("AIMBOT")][xorstr_("selectedPrio")].get<int>();

    // Load ESP settings
    CONFIG::ESP::EnableBoxESP = j[xorstr_("ESP")][xorstr_("EnableBoxESP")].get<bool>();
    CONFIG::ESP::VisibleOnly = j[xorstr_("ESP")][xorstr_("VisibleOnly")].get<bool>();
    CONFIG::ESP::RoundedBox = j[xorstr_("ESP")][xorstr_("RoundedBox")].get<float>();
    CONFIG::ESP::on_key = j[xorstr_("ESP")][xorstr_("on_key")].get<bool>();
    CONFIG::ESP::selectedKey = j[xorstr_("ESP")][xorstr_("selectedKey")].get<int>();
    CONFIG::ESP::EanbleHealthbar = j[xorstr_("ESP")][xorstr_("EnableHealthbar")].get<bool>();
    CONFIG::ESP::EnableSkeletonESP = j[xorstr_("ESP")][xorstr_("EnableSkeletonESP")].get<bool>();
    CONFIG::ESP::SkeletonThickness = j[xorstr_("ESP")][xorstr_("SkeletonThickness")].get<float>();
    CONFIG::ESP::SkeletonOutline = j[xorstr_("ESP")][xorstr_("SkeletonOutline")].get<bool>();
    CONFIG::ESP::EnableNameESP = j[xorstr_("ESP")][xorstr_("EnableNameESP")].get<bool>();
    CONFIG::ESP::SnapLine = j[xorstr_("ESP")][xorstr_("SnapLine")].get<bool>();
    CONFIG::ESP::Fillbox = j[xorstr_("ESP")][xorstr_("Fillbox")].get<bool>();
    CONFIG::ESP::GradiantBox = j[xorstr_("ESP")][xorstr_("GradiantBox")].get<bool>();
    CONFIG::ESP::ExpandF = j[xorstr_("ESP")][xorstr_("ExpandF")].get<int>();
    CONFIG::ESP::ThicknessOutline = j[xorstr_("ESP")][xorstr_("ThicknessOutline")].get<int>();
    CONFIG::ESP::BoxSelected = j[xorstr_("ESP")][xorstr_("BoxSelected")].get<int>();

    for (size_t i = 0; i < 4; ++i) {
        CONFIG::ESP::HealthFull[i] = j[xorstr_("ESP")][xorstr_("HealthFull")][i].get<float>();
        CONFIG::ESP::HealthEmpty[i] = j[xorstr_("ESP")][xorstr_("HealthEmpty")][i].get<float>();
    }

    // Load Visible settings
    for (size_t i = 0; i < 4; ++i) {
        CONFIG::ESP::Visible::Outline[i] = j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("Outline")][i].get<float>();
        CONFIG::ESP::Visible::Skeleton[i] = j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("Skeleton")][i].get<float>();
        CONFIG::ESP::Visible::Name[i] = j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("Name")][i].get<float>();
        CONFIG::ESP::Visible::SnapLine[i] = j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("SnapLine")][i].get<float>();
        CONFIG::ESP::Visible::FillColor1[i] = j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("FillColor1")][i].get<float>();
        CONFIG::ESP::Visible::FillColor2[i] = j[xorstr_("ESP")][xorstr_("Visible")][xorstr_("FillColor2")][i].get<float>();
    }

    // Load InVisible settings
    for (size_t i = 0; i < 4; ++i) {
        CONFIG::ESP::InVisible::Outline[i] = j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("Outline")][i].get<float>();
        CONFIG::ESP::InVisible::Skeleton[i] = j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("Skeleton")][i].get<float>();
        CONFIG::ESP::InVisible::Name[i] = j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("Name")][i].get<float>();
        CONFIG::ESP::InVisible::SnapLine[i] = j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("SnapLine")][i].get<float>();
        CONFIG::ESP::InVisible::FillColor1[i] = j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("FillColor1")][i].get<float>();
        CONFIG::ESP::InVisible::FillColor2[i] = j[xorstr_("ESP")][xorstr_("InVisible")][xorstr_("FillColor2")][i].get<float>();
    }

    inFile.close();
}