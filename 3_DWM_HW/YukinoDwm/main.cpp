#include "main.h"
#include "AsmHook.h"
#include "Utils.h"
#include "overlay/render/draw.hpp"


UINT64 PresentCall = 0;

extern "C" void Present(IDXGISwapChain* SwapChain) {
	SPOOF_FUNC;

	if (!ImGui::GetCurrentContext()) {

		if (SUCCEEDED(SwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice))) {

			g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);

			if (g_pd3dDeviceContext) {

				ID3D11Texture2D* RenderTargetTexture = 0;

				if (SUCCEEDED(SwapChain->GetBuffer(0, IID_PPV_ARGS(&RenderTargetTexture)))) {

					if (SUCCEEDED(g_pd3dDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &g_mainRenderTargetView))) {

						ImGui::CreateContext();

						ImGui_ImplWin32_Init(GetDesktopWindow());

						ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

						ImGuiIO& io = ImGui::GetIO();

						io.IniFilename = NULL;

						io.LogFilename = NULL;

						ImGui::StyleColorsLight();
					}
					RenderTargetTexture->Release();
				}
			}
		}
	}
	if (ImGui::GetCurrentContext()) {

		POINT TempPoint = { 0 };

		ClientToScreen(GetDesktopWindow(), &TempPoint);

		POINT CursorPos = { 0 };

		GetCursorPos(&CursorPos);

		ImGuiIO& io = ImGui::GetIO();

		io.MousePos = ImVec2(CursorPos.x - TempPoint.x, CursorPos.y - TempPoint.y);

		io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

		ImGui_ImplDX11_NewFrame();

		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();

		drawLoop();

		ImGui::Render();

		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void DllThread() {
	SPOOF_FUNC;
	while (TRUE) {
		try {
			int PIDr = Driver->FindProcessID(xorstr_("cs2.exe"));
			if (PIDr != Driver->ProcessID || Driver->ProcessID == NULL || PIDr == NULL) {
				Driver->BaseAddress = NULL;
				Driver->ClientDLL = NULL;
				Driver->ProcessID = NULL;

				if (PIDr != NULL) {
					Sleep(20000);
					Driver->ProcessID = PIDr;
					Driver->CacheCR3();
					Sleep(200);
					uintptr_t Base = Driver->FindBaseAddress();
					Sleep(200);
					uintptr_t Client = Driver->FindModuleAddress(xorstr_("client.dll"));
					Sleep(200);

					if (Base != NULL && Client != NULL && PIDr != NULL) {
						Driver->ProcessID = PIDr;
						Driver->BaseAddress = Base;
						Driver->ClientDLL = Client;
					}
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

bool DllThreadRunning = FALSE;
BOOL InitDwm() {
	SPOOF_FUNC;

	while (Driver->Init() == FALSE) {
		Sleep(1000);
	}

	if (DllThreadRunning == FALSE) {
		std::thread q(DllThread); q.detach();
		DllThreadRunning = TRUE;
	}

	auto d2d1 = (UINT64)GetModuleHandleA("d2d1.dll");

	auto DrawingContext = FindPatternImage(d2d1, "\x48\x8D\x05\x00\x00\x00\x00\x33\xED\x48\x8D\x71\x08", "xxx????xxxxxx");

	if (!DrawingContext) return FALSE;

	DrawingContext += + *(int*)(DrawingContext + 3) + 7;

	auto PresentMultiplaneOverlay = ((UINT64*)DrawingContext)[7];

	auto LdrpDispatchUserCallTarget = FindPattern(PresentMultiplaneOverlay, 0x50, "\xFF\x15", "xx");

	if (!LdrpDispatchUserCallTarget) return FALSE;

	PresentCall = LdrpDispatchUserCallTarget + 6;

	LdrpDispatchUserCallTarget += +*(int*)(LdrpDispatchUserCallTarget + 2) + 6;

	auto Protect = 0ul;

	if (VirtualProtect((PVOID)LdrpDispatchUserCallTarget, 8, PAGE_READWRITE, &Protect)) {

		_InterlockedExchangePointer((PVOID*)LdrpDispatchUserCallTarget, AsmLdrpDispatchUserCallTarget);

		return VirtualProtect((PVOID)LdrpDispatchUserCallTarget, 8, Protect, &Protect);
	}
	return TRUE;
}

BOOL DllMain(HINSTANCE HInstance, UINT32 Reason, PVOID Reserved) {
	SPOOF_FUNC;

	if (Reason == DLL_PROCESS_ATTACH) InitDwm();

	return TRUE;
}