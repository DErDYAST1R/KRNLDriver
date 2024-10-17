#define NOMINMAX
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

#include "../../Driver/API/Driver.h"
#include "WinUser.h"

#include <thread>
#include <cmath>

#include "../../Lib/json.hpp"
#include <D3DX11tex.h>

#include <psapi.h>
#include <tchar.h>

#include "../../Utils/pimraryXor.h"
#include "drawUtils.hpp"
#include <algorithm>
#include <Windows.h>

#include "../../sdk/sdk.h"
#include "framework/framework.hpp"

#include "framework/cfg.h"

void MxInput(int dx, int dy) {
    INPUT buffer;
    ZeroMemory(&buffer, sizeof(buffer));
    buffer.type = INPUT_MOUSE;
    buffer.mi.dx = dx;
    buffer.mi.dy = dy;
    buffer.mi.mouseData = 0;
    buffer.mi.dwFlags = MOUSEEVENTF_MOVE;
    buffer.mi.time = 0;
    buffer.mi.dwExtraInfo = 0;
    SendInput(1, &buffer, sizeof(INPUT));
}

int rannum(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(a, b);
    int val = 0;
    val = distr(gen);
    return val;
}

void RCSThread() {
    int pShotsFired = 0;
    while (true) {
        if (Driver->BaseAddress != NULL) {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                LocalPlayer LocalObject;
                if (LocalObject.LocalPlayerPtr == NULL) continue;
                World worldobj;
                if (VisCheckFrame::LocalParsedMap != worldobj.GetMapName()) {
                    VisCheckFrame::map.unload();
                    VisCheckFrame::map.load_map(std::string("C:\\" + worldobj.GetMapName()));
                    VisCheckFrame::LocalParsedMap = worldobj.GetMapName();
                }

                if (LocalObject.m_iShotsFired >= 1 && CONFIG::AIMBOT::EnableRecoilReducer) {
                    if (LocalObject.m_iShotsFired != pShotsFired && LocalObject.m_iShotsFired >= 2) {
                        pShotsFired = LocalObject.m_iShotsFired;

                        int HumanizationVal = rannum(0, 2);
                        int Side = rannum(0, 1);

                        if (Side == 1)
                            HumanizationVal = -HumanizationVal;

                        if (pShotsFired < 8) {
                            if (GetAsyncKeyState(VK_LBUTTON)) {
                                MxInput(HumanizationVal, CONFIG::AIMBOT::RCSamt);
                            }
                        }
                    }
                }
                else {
                    pShotsFired = 0;
                }
            }
            catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                Sleep(5000);
            }
            catch (...) {
                Sleep(5000);
            }
        }
    }
}

static Vector2 GetCoords2d(Vector3 position) {
    int currentPointerX = 0;
    int currentPointerY = 0;

    int realPositionX = GetSystemMetrics(SM_CXSCREEN) / 2;
    int realPositionY = GetSystemMetrics(SM_CYSCREEN) / 2;

    int correctionX = abs(position.x - realPositionX);
    int correctionY = abs(position.y - realPositionY);

    if (position.x < realPositionX) {
        correctionX = -correctionX;
    }

    if (position.y < realPositionY) {
        correctionY = -correctionY;
    }
    Vector2 coords;
    coords.x = correctionX;
    coords.y = correctionY;

    return coords;
}

void SmoothMouseMove(Vector2 coords, Vector2 Screensense, float SpeedX, float SpeedY) {
    const int incrementTimeMs = 1; // time in milliseconds
    const float distance = std::hypot(coords.x, coords.y);
    const int totalStepsX = static_cast<int>(distance / SpeedX); // adjust for your desired step size
    const int totalStepsY = static_cast<int>(distance / SpeedY); // adjust for your desired step size

    const Vector2 step = { coords.x / totalStepsX, coords.y / totalStepsY };

    if (distance <= 1) {
        MxInput(coords.x, coords.y);
    }

    for (int i = 0; i < totalStepsX; ++i) {
        MxInput(step.x, step.y);
        std::this_thread::sleep_for(std::chrono::milliseconds(incrementTimeMs));
    }

    // Handle any remaining distance
    float remainingX = coords.x - (step.x * totalStepsX);
    float remainingY = coords.y - (step.y * totalStepsY);

    if (std::abs(remainingX) > 0 || std::abs(remainingY) > 0) {
        MxInput(remainingX, remainingY);
    }
}

int SetFOV = 0;
void AimbotThread() {
    int pCacheShotsFired = 0;
    while (TRUE) {
        if (Driver->BaseAddress != NULL && CONFIG::AIMBOT::EnableAimbot) {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                float centerX = GetSystemMetrics(SM_CXSCREEN) / 2.0f;
                float centerY = GetSystemMetrics(SM_CYSCREEN) / 2.0f;
                for (Player PlayerObject : PlayerBuffer) {
                    if (PlayerObject.entity == NULL) continue;
                    World World;
                    LocalPlayer LocalObject;
                    if (PlayerObject.entity == LocalObject.LocalPlayerPtr) continue;
                    if (PlayerObject.m_iHealth <= 0) continue;

                    Vector Start = Driver->RPM<Vector>(LocalObject.LocalPlayerPtr + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
                    Vector End = Driver->RPM<Vector>(PlayerObject.entity + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
                    bool VISstatus = VisCheckFrame::map.is_visible(Start, End);
                    if (CONFIG::AIMBOT::VisibleOnly == TRUE && !VISstatus) {
                        if (PlayerObject.m_iSpotted != TRUE) {
                            continue;
                        }
                    }

                    Vector3 Temp;
                    if (CONFIG::AIMBOT::AimbotKey && !GetAsyncKeyState(CONFIG::AIMBOT::virtualKeyCodes[CONFIG::AIMBOT::selectedKey])) {
                        continue;
                    }

                    if (!CONFIG::AIMBOT::AimAtTeam && PlayerObject.m_iTeamNum == LocalObject.m_iTeamNum) {
                        continue;
                    }

                    if (CONFIG::AIMBOT::StopAfterBullet <= LocalObject.m_iShotsFired) {
                        continue;
                    }

                    if (CONFIG::AIMBOT::OnFirstShot && LocalObject.m_iShotsFired >= 1) {
                        continue;
                    }

                    float SpeedX = CONFIG::AIMBOT::SpeedX;
                    float SpeedY = CONFIG::AIMBOT::SpeedY;

                    SetFOV = CONFIG::AIMBOT::FOV;
                    if (CONFIG::AIMBOT::ShootSettings && CONFIG::AIMBOT::ShootingFOVAfterBullets >= LocalObject.m_iShotsFired) {
                        SetFOV = CONFIG::AIMBOT::ShootingFOV;
                        SpeedX = CONFIG::AIMBOT::ShootingSpeedX;
                        SpeedY = CONFIG::AIMBOT::ShootingSpeedY;
                    }


                    Vector3 bones[] = {
                        PlayerObject.Bones2D.Ahead,
                        PlayerObject.Bones2D.Acou,
                        PlayerObject.Bones2D.AshoulderR,
                        PlayerObject.Bones2D.AshoulderL,
                        PlayerObject.Bones2D.AbrasR,
                        PlayerObject.Bones2D.AbrasL,
                        PlayerObject.Bones2D.AhandR,
                        PlayerObject.Bones2D.AhandL,
                        PlayerObject.Bones2D.Acock,
                        PlayerObject.Bones2D.AkneesR,
                        PlayerObject.Bones2D.AkneesL,
                        PlayerObject.Bones2D.AfeetR,
                        PlayerObject.Bones2D.AfeetL
                    };

                    std::vector<Vector3>(BonesInFOV);
                    for (Vector3 bone : bones) {
                        if (bone.x - centerX < SetFOV && centerX - bone.x < SetFOV && centerY - bone.y < SetFOV && bone.y - centerY < SetFOV) {
                            BonesInFOV.push_back(bone);
                        }
                    }

                    int distance = 99999999;
                    Vector3 ClosestBone;
                    for (Vector3 bone : BonesInFOV) {
                        int dst = bone.Distance(Vector3(centerX, centerY, 0.0f));
                        if (dst < distance) {
                            distance = dst;
                            ClosestBone = bone;
                        }
                    }

                    Vector3 TargetBone;
                    Vector3 ChestBone = bones[8]; ChestBone.y - 7;
                    Vector3 NeckBone = bones[1]; NeckBone.x = bones[8].x;

                    // Priority Bones
                    if (CONFIG::AIMBOT::selectedHitbox == 1) {
                        if (ClosestBone.x - centerX < SetFOV && centerX - ClosestBone.x < SetFOV && centerY - ClosestBone.y < SetFOV && ClosestBone.y - centerY < SetFOV)
                            TargetBone = ClosestBone;
                    }

                    if (CONFIG::AIMBOT::selectedHitbox == 0) {
                        if (ClosestBone.x - centerX < SetFOV && centerX - ClosestBone.x < SetFOV && centerY - ClosestBone.y < SetFOV && ClosestBone.y - centerY < SetFOV)
                            TargetBone = ClosestBone;

                        if (ChestBone.x - centerX < SetFOV && centerX - ChestBone.x < SetFOV && centerY - ChestBone.y < SetFOV && ChestBone.y - centerY < SetFOV)
                            TargetBone = ChestBone;

                        if (NeckBone.x - centerX < SetFOV && centerX - NeckBone.x < SetFOV && centerY - NeckBone.y < SetFOV && NeckBone.y - centerY < SetFOV)
                            TargetBone = NeckBone;
                    }

                    if (CONFIG::AIMBOT::selectedPrio == 1 or CONFIG::AIMBOT::selectedHitbox == 2) {
                        if (bones[0].x - centerX < SetFOV && centerX - bones[0].x < SetFOV && centerY - bones[0].y < SetFOV && bones[0].y - centerY < SetFOV) {
                            TargetBone = bones[0];
                        }
                    }

                    if (CONFIG::AIMBOT::selectedPrio == 2 or CONFIG::AIMBOT::selectedHitbox == 3) {
                        if (NeckBone.x - centerX < SetFOV && centerX - NeckBone.x < SetFOV && centerY - NeckBone.y < SetFOV && NeckBone.y - centerY < SetFOV)
                            TargetBone = NeckBone;
                    }

                    if (CONFIG::AIMBOT::selectedPrio == 3 or CONFIG::AIMBOT::selectedHitbox == 4) {
                        if (ChestBone.x - centerX < SetFOV && centerX - ChestBone.x < SetFOV && centerY - ChestBone.y < SetFOV && ChestBone.y - centerY < SetFOV)
                            TargetBone = ChestBone;
                    }

                    if (TargetBone.x - centerX < SetFOV && centerX - TargetBone.x < SetFOV && centerY - TargetBone.y < SetFOV && TargetBone.y - centerY < SetFOV && TargetBone.x != NULL && TargetBone.y != NULL) {
                        Vector2 coords = GetCoords2d(TargetBone);
                        SmoothMouseMove(coords, Vector2(centerX, centerY), SpeedX, SpeedY);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                Sleep(5000);
            }
            catch (...) {
                Sleep(5000);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int EntityThreadR = 0;
void HackThread(ImDrawList* drawList) {
    if (EntityThreadR == 0) {
        std::thread b(BonesThread); b.detach();
        std::thread t(EntityThread); t.detach();
        std::thread n(RCSThread); n.detach();
        std::thread h(AimbotThread); h.detach();
        EntityThreadR = 1;
    }

    LocalPlayer LocalObject;
    if (CONFIG::AIMBOT::ShowFov) {
        if (LocalObject.m_iShotsFired <= CONFIG::AIMBOT::ShootingFOVAfterBullets) {
            if (CONFIG::AIMBOT::FillFov) {
                overlay::DrawFilledCircle2D(drawList, ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2), CONFIG::AIMBOT::FOV, Float4ToImColor(CONFIG::AIMBOT::FovCircleColor), Float4ToImColor(CONFIG::AIMBOT::FovFilledCircleColor));
            }
            else {
                overlay::DrawCircle2D(drawList, ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2), CONFIG::AIMBOT::FOV, Float4ToImColor(CONFIG::AIMBOT::FovCircleColor));
            }
        }
        if (LocalObject.m_iShotsFired >= (CONFIG::AIMBOT::ShootingFOVAfterBullets + 1) && CONFIG::AIMBOT::ShootSettings) {
            if (CONFIG::AIMBOT::FillFov) {
                overlay::DrawFilledCircle2D(drawList, ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2), CONFIG::AIMBOT::ShootingFOV, Float4ToImColor(CONFIG::AIMBOT::FovCircleColor), Float4ToImColor(CONFIG::AIMBOT::FovFilledCircleColor));
            }
            else {
                overlay::DrawCircle2D(drawList, ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2), CONFIG::AIMBOT::ShootingFOV, Float4ToImColor(CONFIG::AIMBOT::FovCircleColor));
            }
        }
    }

    World World;
    Camera CameraObject;
    for (Player PlayerObject : PlayerBuffer) {
        if (PlayerObject.entity == NULL) continue;
        if (PlayerObject.entity == LocalObject.LocalPlayerPtr) continue;

        Vector3 absorigin;
        CameraObject.w2s(PlayerObject.origin, absorigin, CameraObject.GetViewMatrix());

        Vector3 bones[] = {
            PlayerObject.Bones2D.Ahead,
            PlayerObject.Bones2D.Acou,
            PlayerObject.Bones2D.AshoulderR,
            PlayerObject.Bones2D.AshoulderL,
            PlayerObject.Bones2D.AbrasR,
            PlayerObject.Bones2D.AbrasL,
            PlayerObject.Bones2D.AhandR,
            PlayerObject.Bones2D.AhandL,
            PlayerObject.Bones2D.Acock,
            PlayerObject.Bones2D.AkneesR,
            PlayerObject.Bones2D.AkneesL,
            PlayerObject.Bones2D.AfeetR,
            PlayerObject.Bones2D.AfeetL
        };

        bool hasZeroBone = false;
        for (const auto& bone : bones) {
            if (bone.x == 0 or bone.y == 0 or bone.x == GetSystemMetrics(SM_CXSCREEN) or bone.y == GetSystemMetrics(SM_CYSCREEN)) {
                hasZeroBone = true;
                break;
            }

            if (bone.y < bones[0].y) {
                hasZeroBone = true;
                break;
            }
        }
        if (hasZeroBone) {
            continue;
        }

        Vector3 mostLeftBone;
        Vector3 mostRightBone;
        if (sizeof(bones) / sizeof(bones[0]) > 0) {
            mostLeftBone = bones[0];
            mostRightBone = bones[0];
            for (const auto& bone : bones) {
                if (bone.x < mostLeftBone.x) mostLeftBone = bone;
                if (bone.x > mostRightBone.x) mostRightBone = bone;
            }
        }

        Vector3 mostUpBone;
        Vector3 mostDownBone;
        if (sizeof(bones) / sizeof(bones[0]) > 0) {
            mostUpBone = bones[0];
            mostDownBone = bones[0];
            for (const auto& bone : bones) {
                if (bone.y < mostUpBone.y) mostUpBone = bone;
                if (bone.y > mostDownBone.y) mostDownBone = bone;
            }
        }


        if (PlayerObject.Bones2D.Ahead.y > PlayerObject.Bones2D.AfeetL.y)
            continue; // For clamping

        if (CONFIG::ESP::on_key == TRUE && !GetAsyncKeyState(CONFIG::ESP::virtualKeyCodes[CONFIG::ESP::selectedKey])) {
            continue;
        }

        Vector Start = Driver->RPM<Vector>(LocalObject.LocalPlayerPtr + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
        Vector End = Driver->RPM<Vector>(PlayerObject.entity + schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
        bool VISstatus = VisCheckFrame::map.is_visible(Start, End);
        if (CONFIG::ESP::VisibleOnly == TRUE && !VISstatus) {
            if (PlayerObject.m_iSpotted != TRUE) {
                continue;
            }
        }

        if (CONFIG::ESP::SnapLine) {
            if (VISstatus) {
                overlay::DrawLine2D(drawList, ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, 0), ImVec2(PlayerObject.Bones2D.Ahead.x, PlayerObject.Bones2D.Ahead.y - CONFIG::ESP::ExpandF), Float4ToImColor(CONFIG::ESP::Visible::SnapLine), 1.0f);
            }
            else {
                overlay::DrawLine2D(drawList, ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, 0), ImVec2(PlayerObject.Bones2D.Ahead.x, PlayerObject.Bones2D.Ahead.y - CONFIG::ESP::ExpandF), Float4ToImColor(CONFIG::ESP::InVisible::SnapLine), 1.0f);
            }
        }

        if (CONFIG::ESP::EnableNameESP) {
            if (VISstatus) {
                overlay::DrawText2D(drawList, ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF, mostUpBone.y - 13 - CONFIG::ESP::ExpandF), 11.0f, Float4ToImColor(CONFIG::ESP::Visible::Name), PlayerObject.name.c_str());
            }
            else {
                overlay::DrawText2D(drawList, ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF, mostUpBone.y - 13 - CONFIG::ESP::ExpandF), 11.0f, Float4ToImColor(CONFIG::ESP::InVisible::Name), PlayerObject.name.c_str());
            }
        }

        if (CONFIG::ESP::EanbleHealthbar) {
            overlay::DrawFillBar2D(drawList, ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF - 7, mostUpBone.y - CONFIG::ESP::ExpandF), ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF - 3, mostDownBone.y + CONFIG::ESP::ExpandF), Float4ToImColor(CONFIG::ESP::HealthFull), Float4ToImColor(CONFIG::ESP::HealthEmpty), ImColor(0, 0, 0, 160), (double)(PlayerObject.m_iHealth / 100.0), CONFIG::ESP::ThicknessOutline);
        }


        if (CONFIG::ESP::EnableBoxESP) {
            if (CONFIG::ESP::BoxSelected == 2) {
                if (VISstatus) {
                    overlay::DrawGradientBox2D(drawList,
                        ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF, mostUpBone.y - CONFIG::ESP::ExpandF),
                        ImVec2(mostRightBone.x + CONFIG::ESP::ExpandF, mostDownBone.y + CONFIG::ESP::ExpandF),
                        Float4ToImColor(CONFIG::ESP::Visible::FillColor1), Float4ToImColor(CONFIG::ESP::Visible::FillColor2), Float4ToImColor(CONFIG::ESP::Visible::Outline), CONFIG::ESP::ThicknessOutline);
                }
                else {
                    overlay::DrawGradientBox2D(drawList,
                        ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF, mostUpBone.y - CONFIG::ESP::ExpandF),
                        ImVec2(mostRightBone.x + CONFIG::ESP::ExpandF, mostDownBone.y + CONFIG::ESP::ExpandF),
                        Float4ToImColor(CONFIG::ESP::InVisible::FillColor1), Float4ToImColor(CONFIG::ESP::InVisible::FillColor2), Float4ToImColor(CONFIG::ESP::InVisible::Outline), CONFIG::ESP::ThicknessOutline);
                }
            }

            if (CONFIG::ESP::BoxSelected == 1) {
                if (VISstatus) {
                    overlay::DrawFilledBox2D(drawList,
                        ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF, mostUpBone.y - CONFIG::ESP::ExpandF),
                        ImVec2(mostRightBone.x + CONFIG::ESP::ExpandF, mostDownBone.y + CONFIG::ESP::ExpandF),
                        Float4ToImColor(CONFIG::ESP::Visible::Outline), Float4ToImColor(CONFIG::ESP::Visible::FillColor2), CONFIG::ESP::RoundedBox, CONFIG::ESP::ThicknessOutline);
                }
                else {
                    overlay::DrawFilledBox2D(drawList,
                        ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF, mostUpBone.y - CONFIG::ESP::ExpandF),
                        ImVec2(mostRightBone.x + CONFIG::ESP::ExpandF, mostDownBone.y + CONFIG::ESP::ExpandF),
                        Float4ToImColor(CONFIG::ESP::InVisible::Outline), Float4ToImColor(CONFIG::ESP::InVisible::FillColor2), CONFIG::ESP::RoundedBox, CONFIG::ESP::ThicknessOutline);
                }
            }

            if (CONFIG::ESP::BoxSelected == 0) {
                if (VISstatus) {
                    overlay::DrawBox2D(drawList,
                        ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF, mostUpBone.y - CONFIG::ESP::ExpandF),
                        ImVec2(mostRightBone.x + CONFIG::ESP::ExpandF, mostDownBone.y + CONFIG::ESP::ExpandF),
                        Float4ToImColor(CONFIG::ESP::Visible::Outline), CONFIG::ESP::RoundedBox, CONFIG::ESP::ThicknessOutline);
                }
                else {
                    overlay::DrawBox2D(drawList,
                        ImVec2(mostLeftBone.x - CONFIG::ESP::ExpandF, mostUpBone.y - CONFIG::ESP::ExpandF),
                        ImVec2(mostRightBone.x + CONFIG::ESP::ExpandF, mostDownBone.y + CONFIG::ESP::ExpandF),
                        Float4ToImColor(CONFIG::ESP::InVisible::Outline), CONFIG::ESP::RoundedBox, CONFIG::ESP::ThicknessOutline);
                }
            }
        }

        if (CONFIG::ESP::EnableSkeletonESP) {
            if (VISstatus) {
                // Head to Neck
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Ahead.x, PlayerObject.Bones2D.Ahead.y), ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Neck to Shoulders
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y), ImVec2(PlayerObject.Bones2D.AshoulderR.x, PlayerObject.Bones2D.AshoulderR.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y), ImVec2(PlayerObject.Bones2D.AshoulderL.x, PlayerObject.Bones2D.AshoulderL.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Shoulders to Elbows
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AshoulderR.x, PlayerObject.Bones2D.AshoulderR.y), ImVec2(PlayerObject.Bones2D.AbrasR.x, PlayerObject.Bones2D.AbrasR.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AshoulderL.x, PlayerObject.Bones2D.AshoulderL.y), ImVec2(PlayerObject.Bones2D.AbrasL.x, PlayerObject.Bones2D.AbrasL.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Elbows to Hands
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AbrasR.x, PlayerObject.Bones2D.AbrasR.y), ImVec2(PlayerObject.Bones2D.AhandR.x, PlayerObject.Bones2D.AhandR.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AbrasL.x, PlayerObject.Bones2D.AbrasL.y), ImVec2(PlayerObject.Bones2D.AhandL.x, PlayerObject.Bones2D.AhandL.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Neck to Pelvis
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y), ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Pelvis to Knees
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y), ImVec2(PlayerObject.Bones2D.AkneesR.x, PlayerObject.Bones2D.AkneesR.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y), ImVec2(PlayerObject.Bones2D.AkneesL.x, PlayerObject.Bones2D.AkneesL.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Knees to Feet
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AkneesR.x, PlayerObject.Bones2D.AkneesR.y), ImVec2(PlayerObject.Bones2D.AfeetR.x, PlayerObject.Bones2D.AfeetR.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AkneesL.x, PlayerObject.Bones2D.AkneesL.y), ImVec2(PlayerObject.Bones2D.AfeetL.x, PlayerObject.Bones2D.AfeetL.y), Float4ToImColor(CONFIG::ESP::Visible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

            }
            else {
                // Head to Neck
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Ahead.x, PlayerObject.Bones2D.Ahead.y), ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Neck to Shoulders
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y), ImVec2(PlayerObject.Bones2D.AshoulderR.x, PlayerObject.Bones2D.AshoulderR.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y), ImVec2(PlayerObject.Bones2D.AshoulderL.x, PlayerObject.Bones2D.AshoulderL.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Shoulders to Elbows
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AshoulderR.x, PlayerObject.Bones2D.AshoulderR.y), ImVec2(PlayerObject.Bones2D.AbrasR.x, PlayerObject.Bones2D.AbrasR.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AshoulderL.x, PlayerObject.Bones2D.AshoulderL.y), ImVec2(PlayerObject.Bones2D.AbrasL.x, PlayerObject.Bones2D.AbrasL.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Elbows to Hands
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AbrasR.x, PlayerObject.Bones2D.AbrasR.y), ImVec2(PlayerObject.Bones2D.AhandR.x, PlayerObject.Bones2D.AhandR.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AbrasL.x, PlayerObject.Bones2D.AbrasL.y), ImVec2(PlayerObject.Bones2D.AhandL.x, PlayerObject.Bones2D.AhandL.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Neck to Pelvis
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acou.x, PlayerObject.Bones2D.Acou.y), ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Pelvis to Knees
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y), ImVec2(PlayerObject.Bones2D.AkneesR.x, PlayerObject.Bones2D.AkneesR.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.Acock.x, PlayerObject.Bones2D.Acock.y), ImVec2(PlayerObject.Bones2D.AkneesL.x, PlayerObject.Bones2D.AkneesL.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

                // Knees to Feet
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AkneesR.x, PlayerObject.Bones2D.AkneesR.y), ImVec2(PlayerObject.Bones2D.AfeetR.x, PlayerObject.Bones2D.AfeetR.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);
                overlay::DrawLine2D(drawList, ImVec2(PlayerObject.Bones2D.AkneesL.x, PlayerObject.Bones2D.AkneesL.y), ImVec2(PlayerObject.Bones2D.AfeetL.x, PlayerObject.Bones2D.AfeetL.y), Float4ToImColor(CONFIG::ESP::InVisible::Skeleton), CONFIG::ESP::SkeletonThickness, CONFIG::ESP::SkeletonOutline);

            }


        }

    }
}

void DrawOverlay() {
    SPOOF_FUNC;
    ImGui::Begin(xorstr_("renderer"), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs);
    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
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
    style->Colors[ImGuiCol_Border] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_BorderShadow] = ImColor(57, 57, 57);
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
    style->Colors[ImGuiCol_Button] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.21f, 0.09f, 0.27f, 0.80f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.00f, 0.54f, 1.00f);
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
    style->Colors[ImGuiCol_Tab] = ImColor(255, 182, 193);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(0.21f, 0.09f, 0.27f, 0.80f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.49f, 0.00f, 0.54f, 1.00f);
    style->TabRounding = 0.0f;
    style->WindowRounding = 5.0f;
    style->FrameRounding = 4.0f;
    style->FramePadding = ImVec2(16.0f, 3.0f);
    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;
}

bool LoadedStyles = FALSE;
bool ShowMenu = FALSE;
void DrawMenu() {
    SPOOF_FUNC;

    if (GetAsyncKeyState(VK_INSERT) & 1) {
        ShowMenu = !ShowMenu;
    }

    if (ShowMenu) {
        if (ImGui::Begin(xorstr_("XIMAMenu"), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
        {
            // ImDrawList* drawList = ImGui::GetWindowDrawList();

            ImGui::SetWindowPos(ImVec2(15.0f, 45.0f), ImGuiCond_Always);

            // Create a tab bar
            if (ImGui::BeginTabBar(xorstr_("dk28722"))) {
                if (ImGui::BeginTabItem(xorstr_("Aimbot"))) {
                    ImGui::Checkbox(xorstr_("Enable Global Aimbot"), &CONFIG::AIMBOT::EnableAimbot);
                    ImGui::SeparatorText(xorstr_("Visual"));
                    ImGui::Checkbox(xorstr_("Show FOV"), &CONFIG::AIMBOT::ShowFov);
                    ImGui::SameLine(); ImGui::ColorEdit4("428272", CONFIG::AIMBOT::FovCircleColor, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::Checkbox(xorstr_("Show Filled FOV"), &CONFIG::AIMBOT::FillFov);
                    ImGui::SameLine(); ImGui::ColorEdit4("492722", CONFIG::AIMBOT::FovFilledCircleColor, true | 128 | 256 | 32 | 512 | 256);

                    ImGui::SeparatorText(xorstr_("Misc"));
                    ImGui::Checkbox(xorstr_("Visible Only"), &CONFIG::AIMBOT::VisibleOnly);
                    ImGui::Combo(xorstr_("Hitbox"), &CONFIG::AIMBOT::selectedHitbox, CONFIG::AIMBOT::hitboxes, IM_ARRAYSIZE(CONFIG::AIMBOT::hitboxes));
                    ImGui::Combo(xorstr_("Priority Hitbox"), &CONFIG::AIMBOT::selectedPrio, CONFIG::AIMBOT::prio, IM_ARRAYSIZE(CONFIG::AIMBOT::prio));
                    ImGui::TextColored(ImVec4(175, 175, 175, 175), xorstr_("Priority Hitbox is for Progression or Nearest when you want to Prio to a specific bone"));
                    ImGui::Spacing();
                    ImGui::Checkbox(xorstr_("Enable Shot Settings"), &CONFIG::AIMBOT::ShootSettings);
                    ImGui::SliderInt(xorstr_("Shot Settings After Bullet"), &CONFIG::AIMBOT::ShootingFOVAfterBullets, 0, 30);
                    ImGui::SliderInt(xorstr_("Shooting FOV"), &CONFIG::AIMBOT::ShootingFOV, 0, 50);
                    ImGui::SliderFloat(xorstr_("Shooting SpeedX"), &CONFIG::AIMBOT::ShootingSpeedX, 1.0f, 5.0f, "%.2f");
                    ImGui::SliderFloat(xorstr_("Shooting SpeedY"), &CONFIG::AIMBOT::ShootingSpeedY, 1.0f, 5.0f, "%.2f");
                    ImGui::SeparatorText(xorstr_("Aimbot Settings"));
                    ImGui::SliderInt(xorstr_("FOV Range"), &CONFIG::AIMBOT::FOV, 0, 50);
                    ImGui::SliderFloat(xorstr_("Aimbot SpeedX"), &CONFIG::AIMBOT::SpeedX, 1.0f, 5.0f, "%.2f");
                    ImGui::SliderFloat(xorstr_("Aimbot SpeedY"), &CONFIG::AIMBOT::SpeedY, 1.0f, 5.0f, "%.2f");
                    ImGui::SliderInt(xorstr_("Stop After Bullet"), &CONFIG::AIMBOT::StopAfterBullet, 0, 30);
                    ImGui::Checkbox(xorstr_("Aimbot Key"), &CONFIG::AIMBOT::AimbotKey);
                    ImGui::Combo(xorstr_("Aim Key"), &CONFIG::AIMBOT::selectedKey, CONFIG::AIMBOT::keys, IM_ARRAYSIZE(CONFIG::AIMBOT::keys));
                    ImGui::Checkbox(xorstr_("After First Shot"), &CONFIG::AIMBOT::OnFirstShot);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(xorstr_("Visuals"))) {
                    ImGui::Checkbox(xorstr_("On-Key"), &CONFIG::ESP::on_key);
                    if (CONFIG::ESP::on_key) {
                        ImGui::Combo(xorstr_("Key"), &CONFIG::ESP::selectedKey, CONFIG::ESP::keys, IM_ARRAYSIZE(CONFIG::ESP::keys));
                    }
                    ImGui::Checkbox(xorstr_("Visible Only"), &CONFIG::ESP::VisibleOnly);
                    ImGui::SliderInt("Scaling / Expand", &CONFIG::ESP::ExpandF, 1, 20);
                    ImGui::Separator();

                    ImGui::Checkbox(xorstr_("Enable Box ESP"), &CONFIG::ESP::EnableBoxESP);
                    ImGui::Combo(xorstr_("Box Style"), &CONFIG::ESP::BoxSelected, CONFIG::ESP::boxes, IM_ARRAYSIZE(CONFIG::ESP::boxes));
                    if (CONFIG::ESP::BoxSelected == 0)
                        ImGui::SliderFloat("Box Rounding", &CONFIG::ESP::RoundedBox, 0.0f, 1.0f, "%.2f");
                    ImGui::SliderInt("Box Thickness", &CONFIG::ESP::ThicknessOutline, 0, 5);

                    ImGui::Checkbox(xorstr_("Enable Skeleton ESP"), &CONFIG::ESP::EnableSkeletonESP);
                    ImGui::Checkbox(xorstr_("Enable Skeleton Outline"), &CONFIG::ESP::SkeletonOutline);
                    ImGui::SliderFloat("Skeleton Thickness", &CONFIG::ESP::SkeletonThickness, 1.0f, 2.5f, "%.2f");

                    ImGui::Checkbox(xorstr_("Enable SnapLine"), &CONFIG::ESP::SnapLine);
                    ImGui::Checkbox(xorstr_("Enable HealthBar"), &CONFIG::ESP::EanbleHealthbar);
                    ImGui::Checkbox(xorstr_("Enable Name"), &CONFIG::ESP::EnableNameESP);
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(xorstr_("Misc"))) {
                    ImGui::Checkbox(xorstr_("Enable RCS Reduce"), &CONFIG::AIMBOT::EnableRecoilReducer);
                    ImGui::SliderInt(xorstr_("Reduce By Pixel(Y)"), &CONFIG::AIMBOT::RCSamt, 0, 10);
                    ImGui::SeparatorText("Config");
                    if (ImGui::Button(xorstr_("Save Config"))) {
                        saveConfig();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(xorstr_("Load Config"))) {
                        loadConfig();
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(xorstr_("Colors"))) {
                    ImGui::Text(xorstr_("Box Visible/Not-Visible"));
                    ImGui::SameLine(); ImGui::ColorEdit4("15431253", CONFIG::ESP::Visible::Outline, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::SameLine(); ImGui::ColorEdit4("497223", CONFIG::ESP::InVisible::Outline, true | 128 | 256 | 32 | 512 | 256);

                    ImGui::Text(xorstr_("Box Fill (Visible)"));
                    ImGui::SameLine(); ImGui::ColorEdit4("5312435", CONFIG::ESP::Visible::FillColor1, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::SameLine(); ImGui::ColorEdit4("5533422", CONFIG::ESP::Visible::FillColor2, true | 128 | 256 | 32 | 512 | 256);

                    ImGui::Text(xorstr_("Box Fill (InVisible)"));
                    ImGui::SameLine(); ImGui::ColorEdit4("3826723", CONFIG::ESP::InVisible::FillColor1, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::SameLine(); ImGui::ColorEdit4("3917222", CONFIG::ESP::InVisible::FillColor2, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::Text(xorstr_("Regular fill box uses FillColor2"));

                    ImGui::Spacing();

                    ImGui::Text(xorstr_("Skeleton Visible/Not-Visible"));
                    ImGui::SameLine(); ImGui::ColorEdit4("5418272", CONFIG::ESP::Visible::Skeleton, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::SameLine(); ImGui::ColorEdit4("438267", CONFIG::ESP::InVisible::Skeleton, true | 128 | 256 | 32 | 512 | 256);

                    ImGui::Text(xorstr_("Health Full/Empty"));
                    ImGui::SameLine(); ImGui::ColorEdit4("542314", CONFIG::ESP::HealthFull, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::SameLine(); ImGui::ColorEdit4("5533432", CONFIG::ESP::HealthEmpty, true | 128 | 256 | 32 | 512 | 256);

                    ImGui::Text(xorstr_("Name Visible/Not-Visible"));
                    ImGui::SameLine(); ImGui::ColorEdit4("321555", CONFIG::ESP::Visible::Name, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::SameLine(); ImGui::ColorEdit4("3526612", CONFIG::ESP::InVisible::Name, true | 128 | 256 | 32 | 512 | 256);

                    ImGui::Text(xorstr_("Snapline Visible/Not-Visible"));
                    ImGui::SameLine(); ImGui::ColorEdit4("6426664", CONFIG::ESP::Visible::SnapLine, true | 128 | 256 | 32 | 512 | 256);
                    ImGui::SameLine(); ImGui::ColorEdit4("65463333", CONFIG::ESP::InVisible::SnapLine, true | 128 | 256 | 32 | 512 | 256);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(xorstr_("Changelog"))) {

                    ImGui::EndTabItem();
                }
                // ImVec2 mousePos = ImGui::GetMousePos();
                // drawList->AddCircleFilled(mousePos, 6.0f, IM_COL32(0, 0, 255, 255), 32);
            }
        }
        ImGui::End();
    }
}

void drawLoop() {
    SPOOF_FUNC;
    ImGuiIO& io = ImGui::GetIO();
    POINT p;
    GetCursorPos(&p);
    io.MousePos.x = p.x;
    io.MousePos.y = p.y;

    if (LoadedStyles == FALSE) {
        RunStyle();
        LoadedStyles = TRUE;
    }

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

        try {
            DrawOverlay();
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            Sleep(5000);
        }
        catch (...) {
            Sleep(5000);
        }

        try {
            DrawMenu();
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            Sleep(5000);
        }
        catch (...) {
            Sleep(5000);
        }
    }

    if (Driver->BaseAddress == NULL) {
        ImGui::SetNextWindowSize({ 225, 56 });
        if (ImGui::Begin(xorstr_("XIMA"), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::SetWindowPos(ImVec2(15.0f, 15.0f), ImGuiCond_Always);

            ImGui::Separator();
            ImGui::Text(xorstr_("Waiting for game window..."));
        }
        ImGui::End();
    }
}