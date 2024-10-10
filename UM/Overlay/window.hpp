// Made by Ítalo Seara (https://github.com/italoseara)

#ifndef OVERLAY_HPP
#define OVERLAY_HPP
#define STB_IMAGE_IMPLEMENTATION

#include "../Lib/fonts.hpp"

#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <iostream>
#include <D3DX11tex.h>

#include <Lmcons.h>
#include <filesystem>

#include "../Lib/imgui/imgui.h"
#include "../Lib/imgui/imgui_impl_dx11.h"
#include "../Lib/imgui/imgui_impl_win32.h"

#include "../Driver/UTILS/skCrypt.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool WroteFonts = false;

namespace Overlay
{
    HINSTANCE hInstance = nullptr;
    INT nCmdShow = 1;
    void (*drawLoop)();

    // Window
    HWND window = nullptr;
    int width = 0;
    int height = 0;

    // DirectX
    IDXGISwapChain* swapChain = nullptr;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;

    D3DX11_IMAGE_LOAD_INFO info;
    ID3DX11ThreadPump* pump{ nullptr };

    LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam))
            return 0L;

        if (message == WM_DESTROY)
        {
            PostQuitMessage(0);
            return 0L;
        }

        return DefWindowProc(window, message, wParam, lParam);
    }

    void InitializeWindow()
    {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProcedure;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"UL";
        if (!RegisterClassExW(&wc))
        {
            std::cerr << E("Failed to register window class.") << std::endl;
            exit(1);
        }

        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
        window = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
            wc.lpszClassName,
            L"U",
            WS_POPUP,
            0, 0,
            width, height,
            nullptr,
            nullptr,
            wc.hInstance,
            nullptr
        );

        if (!window)
        {
            std::cerr << E("Failed to create window.") << std::endl;
            exit(1);
        }

        SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

        {
            RECT clientArea{};
            if (!GetClientRect(window, &clientArea))
            {
                std::cerr << E("Failed to get client area.") << std::endl;
                exit(1);
            }

            RECT windowArea{};
            if (!GetWindowRect(window, &windowArea))
            {
                std::cerr << E("Failed to get window rect.") << std::endl;
                exit(1);
            }

            POINT diff{};
            if (!ClientToScreen(window, &diff))
            {
                std::cerr << E("Failed to convert client to screen.") << std::endl;
                exit(1);
            }

            const MARGINS margins{
                windowArea.left + (diff.x - windowArea.left),
                windowArea.top + (diff.y - windowArea.top),
                clientArea.right,
                clientArea.bottom
            };

            if (FAILED(DwmExtendFrameIntoClientArea(window, &margins)))
            {
                std::cerr << E("Failed to extend frame into client area.") << std::endl;
                exit(1);
            }
        }

        ShowWindow(window, nCmdShow);
        UpdateWindow(window);
    }

    void InitializeDirectX()
    {
        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferDesc.RefreshRate.Numerator = 60U;
        sd.BufferDesc.RefreshRate.Denominator = 1U;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1U;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1U;
        sd.OutputWindow = window;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        constexpr D3D_FEATURE_LEVEL levels[2]{
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0,
        };

        if (FAILED(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0U,
            levels,
            2U,
            D3D11_SDK_VERSION,
            &sd,
            &swapChain,
            &device,
            nullptr,
            &deviceContext)))
        {
            std::cerr << E("Failed to create device and swap chain.") << std::endl;
            exit(1);
        }

        ID3D11Texture2D* backBuffer{ nullptr };
        if (FAILED(swapChain->GetBuffer(0U, IID_PPV_ARGS(&backBuffer))))
        {
            std::cerr << E("Failed to get swap chain buffer.") << std::endl;
            exit(1);
        }

        if (backBuffer)
        {
            if (FAILED(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView)))
            {
                std::cerr << E("Failed to create render target view.") << std::endl;
                exit(1);
            }
            backBuffer->Release();
        }
        else
        {
            std::cerr << E("Failed to get back buffer.") << std::endl;
            exit(1);
        }
    }

    void InitializeImGui()
    {
        if (!ImGui::CreateContext())
        {
            std::cerr << E("Failed to create ImGui context.") << std::endl;
            exit(1);
        }
        ImGui::StyleColorsDark();

        if (!ImGui_ImplWin32_Init(window))
        {
            std::cerr << E("Failed to initialize ImGui Win32.") << std::endl;
            exit(1);
        }
        if (!ImGui_ImplDX11_Init(device, deviceContext))
        {
            std::cerr << E("Failed to initialize ImGui DX11.") << std::endl;
            exit(1);
        }

    }

    void RenderLoop()
    {
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));
        bool running = true;
        

        while (true)
        {
            while (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);

                if (msg.message == WM_QUIT)
                    running = false;
            }

            if (!running) break;

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // Draw your stuff here
            drawLoop();

            ImGui::Render();

            constexpr FLOAT clearColor[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
            deviceContext->OMSetRenderTargets(1U, &renderTargetView, nullptr);
            deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            if (FAILED(swapChain->Present(1U, 0U)))
            {
                std::cerr << E("Failed to present swap chain.") << std::endl;
                exit(1);
            }
        }
    }

    void Cleanup()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if (swapChain)        swapChain->Release();
        if (deviceContext)    deviceContext->Release();
        if (device)           device->Release();
        if (renderTargetView) renderTargetView->Release();

        DestroyWindow(window);
        UnregisterClassW(L"U", hInstance);

        exit(0);
    }


    void fonts() {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontDefault();

        if (!WroteFonts) {
            saveFont();
            Sleep(1000);
            WroteFonts = true;
        }

        TCHAR username[UNLEN + 1];
        DWORD username_len = UNLEN + 1;
        if (!GetUserName(username, &username_len)) {
            return;
        }

        std::string ws(username);
        std::string user_name(ws.begin(), ws.end());

        std::filesystem::path font1_path = ("C:\\Users\\") + user_name + ("\\AppData\\Local\\Microsoft\\Windows\\Fonts\\Tanseek Modern W20 Medium.ttf");
        std::filesystem::path font2_path = ("C:\\Users\\") + user_name + ("\\AppData\\Local\\Microsoft\\Windows\\Fonts\\CheatmenuFont-Regular.ttf");
        std::filesystem::path font3_path = ("C:\\Users\\") + user_name + ("\\AppData\\Local\\Microsoft\\Windows\\Fonts\\Tanseek Modern W20 Medium.ttf");

        if (std::filesystem::exists(font1_path))
            io.Fonts->AddFontFromFileTTF(font1_path.string().c_str(), 18.0f);
        if (std::filesystem::exists(font2_path))
            io.Fonts->AddFontFromFileTTF(font2_path.string().c_str(), 26.0f);
        if (std::filesystem::exists(font3_path))
            io.Fonts->AddFontFromFileTTF(font3_path.string().c_str(), 15.0f);

    }

    void Run(HINSTANCE hInstance, INT nCmdShow, void (*drawLoop)())
    {
        Overlay::hInstance = hInstance;
        Overlay::nCmdShow = nCmdShow;
        Overlay::drawLoop = drawLoop;

        InitializeWindow();
        InitializeDirectX();
        InitializeImGui();

        Overlay::fonts();

        RenderLoop();
        Cleanup();
    }
}

#endif // OVERLAY_HPP