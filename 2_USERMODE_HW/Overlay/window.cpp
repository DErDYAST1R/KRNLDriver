#pragma once
#include "window.hpp"
#include "../Lib/fonts.hpp"
#include "../Driver/API/Driver.h"
bool Overlay::InitDevice()
{
	SPOOF_FUNC;
	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
	swap_chain_description.BufferCount = 2;
	swap_chain_description.BufferDesc.Width = 0;
	swap_chain_description.BufferDesc.Height = 0;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow = window_handle;
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;
	swap_chain_description.Windowed = TRUE;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL feature_level;
	D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_level_array, 2, D3D11_SDK_VERSION, &swap_chain_description, &swap_chain, &d3d_device, &feature_level, &device_context) != S_OK)
		return false;

	InitRenderTarget();

	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);

	return true;
}

void Overlay::DestroyDevice()
{
	SPOOF_FUNC;
	DestroyRenderTarget();
	swap_chain->Release();
	device_context->Release();
	d3d_device->Release();
}

void Overlay::DestroyImGui()
{
	SPOOF_FUNC;
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Overlay::InitRenderTarget()
{
	SPOOF_FUNC;
	ID3D11Texture2D* back_buffer = nullptr;
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	if (back_buffer)
	{
		d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
		back_buffer->Release();
	}
}

void Overlay::DestroyRenderTarget()
{
	SPOOF_FUNC;
	if (!render_target_view)
		return;

	render_target_view->Release();
	render_target_view = NULL;
}

void Overlay::DestroyWindow()
{
	SPOOF_FUNC;
	char command[256];
	snprintf(command, sizeof(command), "taskkill /F /IM \"%s\"", "ow-overlay.exe");
	system(command);
}

void Overlay::InitImGui()
{
	SPOOF_FUNC;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	/* styles & fonts here */
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(window_handle);
	ImGui_ImplDX11_Init(d3d_device, device_context);
}

Overlay::Overlay(HWND target_window)
{
	SPOOF_FUNC;
	window_handle = target_window;
	if (!window_handle)
		return;

	if (!InitDevice())
		return;

	InitImGui();
}

Overlay::~Overlay()
{
	SPOOF_FUNC;
	DestroyImGui();
	DestroyDevice();
}

// const void Overlay::Render()
// {
// 	ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImColor(255, 255, 255, 255), "test");
// }
bool WroteFonts = false;
void fonts() {
	SPOOF_FUNC;
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

bool Overlay::MessageLoop(void (*drawLoop)())
{
	SPOOF_FUNC;
	if (window_handle)
	{
		ImVec4 clear_clr = { 0, 0, 0, 0 };

		fonts();
		while (true)
		{
			try {
				MSG msg{ 0 };

				if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
						return false;

					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				else
				{
					ImGui_ImplDX11_NewFrame();
					ImGui_ImplWin32_NewFrame();
					ImGui::NewFrame();

					drawLoop();

					ImGui::Render();
					device_context->OMSetRenderTargets(1, &render_target_view, NULL);
					device_context->ClearRenderTargetView(render_target_view, (float*)&clear_clr);

					ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

					swap_chain->Present(1, 0);

					if (GetAsyncKeyState(VK_DELETE)) {
						ImGui::End();
						break;
					}
				}
			}
			catch (const std::runtime_error& e) {
				std::cout << "Caught a runtime error: " << e.what() << std::endl;
			}
			catch (const std::out_of_range& e) {
				std::cout << "Caught an out of range error: " << e.what() << std::endl;
			}
			catch (...) {
				std::cout << "Caught an unknown exception." << std::endl;
			}
		}
	}
	return false;
}