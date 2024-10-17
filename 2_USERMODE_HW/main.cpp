#include "Bypass.h"
#include "Driver/API/Driver.h"
#include "utils/pimraryXor.h"
#include "Driver/Callstack-Spoofer.h"

#include "Overlay/Render/draw.hpp"
#include "Overlay/window.hpp"
#include "color.hpp"

#include <cstdlib>
#define CURL_STATICLIB
#include "Lib/curl/curl/curl.h"
#pragma comment(lib, "Lib/curl/curl/libcurl_a.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")

#define executableName "cs2.exe"
#define windowTitle "Counter-Strike 2"
#define driverLocation "C:\\Windows\\System32\\Impl32.sys"
bool DebugRead = TRUE;

void RemoveScrollbar() {
	SPOOF_FUNC;
	int AGEHUGAIUHVAR1 = 1613513513;
	int AGEHUGAIUHVAR2 = 1357981351;
	int AGEHUGAIUHVAR3 = 6135413635;
	int AGEHUGAIUHVAR4 = 1351351515;
	int AGEHUGAIUHVAR5 = 6135135151;

	while (AGEHUGAIUHVAR1 == 13651351)
	{
		AGEHUGAIUHVAR1 += 1;
	}

	while (AGEHUGAIUHVAR2 == 3151351351)
	{
		AGEHUGAIUHVAR2 += 1;
	}

	while (AGEHUGAIUHVAR3 == 136511351135351)
	{
		AGEHUGAIUHVAR3 += 1;
	}

	while (AGEHUGAIUHVAR4 == 13613551351135135)
	{
		AGEHUGAIUHVAR4 += 1;
	}

	while (AGEHUGAIUHVAR5 == 13513515115)
	{
		AGEHUGAIUHVAR5 += 1;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) {
		return;
	}
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		return;
	}
	SMALL_RECT windowSize = csbi.srWindow;
	COORD bufferSize = { windowSize.Right - windowSize.Left + 1, windowSize.Bottom - windowSize.Top + 1 };
	if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) {
		return;
	}
	if (!SetConsoleWindowInfo(hConsole, TRUE, &windowSize)) {
		return;
	}
}

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT nCmdShow)
{
	SPOOF_FUNC;
	int AGEHUGAIUHVAR1 = 1613513513;
	int AGEHUGAIUHVAR2 = 1357981351;
	int AGEHUGAIUHVAR3 = 6135413635;
	int AGEHUGAIUHVAR4 = 1351351515;
	int AGEHUGAIUHVAR5 = 6135135151;

	while (AGEHUGAIUHVAR1 == 13651351)
	{
		AGEHUGAIUHVAR1 += 1;
	}

	while (AGEHUGAIUHVAR2 == 3151351351)
	{
		AGEHUGAIUHVAR2 += 1;
	}

	while (AGEHUGAIUHVAR3 == 136511351135351)
	{
		AGEHUGAIUHVAR3 += 1;
	}

	while (AGEHUGAIUHVAR4 == 13613551351135135)
	{
		AGEHUGAIUHVAR4 += 1;
	}

	while (AGEHUGAIUHVAR5 == 13513515115)
	{
		AGEHUGAIUHVAR5 += 1;
	}

	AllocConsole();
	FILE* fp;
	freopen_s(&fp, E("CONOUT$"), E("w"), stdout);
	
	// std::string TitleV = uniqueName();
	SetConsoleTitleA(E("58236"));
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND consoleApp = GetConsoleWindow();
	
	SetLayeredWindowAttributes(consoleApp, RGB(154, 255, 214), 230, LWA_ALPHA);
	LONG lStyle = GetWindowLong(consoleApp, GWL_STYLE);
	lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_EX_TOOLWINDOW);
	SetWindowLong(consoleApp, GWL_STYLE, lStyle);
	
	HMENU hMenu = GetSystemMenu(consoleApp, FALSE);
	if (hMenu != nullptr)
	{
		RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		DrawMenuBar(consoleApp);
		RemoveScrollbar();
	}
	
	SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	
	std::string startingMessage = std::string(E(" KRNLWare Revamped\n\n"));
	std::string loadingMessage = std::string(E(" Press (F2) To load HWID Spoofer\n"));
	std::string hwidDisabledMessage = std::string(E(" HWID Spoofer Disabled at this time...\n"));
	std::string waitingForProcessMessage = std::string(E("Waiting for ")) + std::string(E(executableName)) + "\n";
	std::string foundProcessMessage = std::string(E("Found ")) + std::string(E(executableName)) + "\n";
	std::string processNotFoundMessage = std::string(E("Process Not Found\n"));
	std::string baseAddressNotFoundMessage = std::string(E("Base Address Not Found\n"));
	std::string waitingForOverlayMessage = std::string(E("Waiting for OverWolf Overlay to Initialize\n"));
	std::string medalOverlayHookedMessage = std::string(E("OverWolf Overlay Hooked!\n"));
	std::string failedToHookOverlayMessage = std::string(E("Failed to hook OverWolf Overlay\n"));
	std::string overlayStartedMessage = std::string(E("Overlay Started...\n"));
	std::string overlayDestroyedMessage = std::string(E("Overlay Destroyed...\n"));
	INT32 curPiD = GetCurrentProcessId();

	BOOL DriverHandle = Driver->Init();
	if (DriverHandle) {
		Bypass::UnloadGhidraDriver(E(driverLocation));
		Sleep(500);
		Bypass::LoadGhidraDriver(E(driverLocation));
		DriverHandle = Driver->Init();
	}
	if (!DriverHandle) {
		Bypass::LoadGhidraDriver(E(driverLocation));
		DriverHandle = Driver->Init();
	}
	
	std::cout << dye::grey(" [+] ") << dye::light_yellow(waitingForProcessMessage);
	Driver->ProcessID = Driver->FindProcessID(xorstr_(executableName));
	while (Driver->ProcessID == NULL) {
		Sleep(2000);
		Driver->ProcessID = Driver->FindProcessID(xorstr_(executableName));
	}
	// LI_FN(ShowWindow)(GetConsoleWindow(), SW_HIDE);
	std::cout << dye::grey(" [+] ") << dye::green(foundProcessMessage);
	Sleep(2000);
	Driver->CacheCR3();
	uintptr_t BaseAddress = Driver->FindBaseAddress();
	Driver->BaseAddress = BaseAddress;
	if (BaseAddress == NULL) {
		std::cout << dye::grey(" [+] ") << dye::red(baseAddressNotFoundMessage);
		Sleep(2000);
		return 1;
	}
	
	Beep(723, 200);
	
	Sleep(1000);
	std::cout << dye::grey(" [+] ") << E(" BaseAddress: : ") << std::hex << Driver->BaseAddress << std::endl;
	std::cout << dye::grey(" [+] ") << E(" __in64@BaseAddress: ") << std::hex << Driver->RPM<__int64>(Driver->BaseAddress) << std::endl;

	std::cout << dye::grey(" [+] ") << dye::light_yellow(waitingForOverlayMessage);
	HWND OWHND = LI_FN(FindWindowA)(E("OOPO_WINDOWS_CLASS"), E("ow overlay"));
	while (!OWHND) {
		Sleep(500);
		OWHND = LI_FN(FindWindowA)(E("OOPO_WINDOWS_CLASS"), E("ow overlay"));
	}

	Driver->ClientDLL = Driver->FindModuleAddress("client.dll");
	Overlay overlay(OWHND);
	if (!overlay.MessageLoop(drawLoop)) {
		std::cout << dye::grey(" [+] ") << dye::red(failedToHookOverlayMessage);
		Sleep(2000);
		exit(1);
	}
	else {
		std::cout << dye::grey(" [+] ") << dye::green(medalOverlayHookedMessage);
	}
	overlay.DestroyImGui();
	overlay.DestroyDevice();

	return 0;
}