#define NOMINMAX
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

#include "../../../Driver/API/Driver.h"
#include "../../../Utils/pimraryXor.h"
#include "../../../sdk/sdk.h"

#include <WinUser.h>
#include <thread>
#include <cmath>
#include <D3DX11tex.h>
#include <psapi.h>
#include <tchar.h>
#include <thread>
#include <algorithm>
#include <Windows.h>

namespace VisCheckFrame {
    map_loader map;
    std::string LocalParsedMap = "";
}

std::vector<Player> PlayerBuffer = {};
void EntityThread() {
    while (true) {
        try {
            if (Driver->BaseAddress != NULL) {
                LocalPlayer LocalObject;
                std::vector<Player> TempBuffer = {};
                for (int i = 1; i <= 128; i++) {
                    if (LocalObject.LocalPlayerPtr == NULL) {
                        PlayerBuffer.clear();
                        continue;
                    }
                    Player PlayerObject(i);
                    if (PlayerObject.entity == NULL) continue;

                    TempBuffer.push_back(PlayerObject);
                }
                PlayerBuffer = TempBuffer;
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
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

void BonesThread() {
    while (TRUE) {
        try {
            if (Driver->BaseAddress != NULL) {
                for (Player& PlayerObject : PlayerBuffer) {
                    if (PlayerObject.entity == NULL) continue;
                    if (PlayerObject.m_iHealth <= 0) continue;
                    Camera CameraObject;
                    if (CameraObject.ViewMatrix.matrix == NULL) continue;
                    PlayerObject.Cache3DBones();
                    PlayerObject.Cache2DBones(CameraObject);
                }
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