#include "Bypass.h"
#include "Driver/API/Driver.h"
#include "utils/pimraryXor.h"

#include "Overlay/draw.hpp"
#include "Overlay/window.hpp"

#include <cstdlib>
#define CURL_STATICLIB
#include "Lib/curl/curl/curl.h"
#pragma comment(lib, "Lib/curl/curl/libcurl_a.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")

void RemoveScrollbar() {
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

std::string uniqueName() {
	auto randchar = []() -> char
		{
			const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
	std::string str(4, 0);
	std::generate_n(str.begin(), 4, randchar);
	return str;
}

void ExecuteCommand(const std::string& command) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	if (CreateProcess(NULL, const_cast<LPSTR>(command.c_str()), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {
		std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
	}
}

void DeletePartition(const std::string& partitionName) {
	std::ofstream script("delete_partition.txt");
	script << "select volume " << partitionName << "\n";
	script << "delete volume\n";
	script.close();

	ExecuteCommand("diskpart /s delete_partition.txt");
	DeleteFile("delete_partition.txt");
}

void CR3Thread() {
	while (true) {
		__int64 RfCheck = Driver->RPM<__int64>(Driver->BaseAddress);
		if (RfCheck == NULL) {
			Driver->CacheCR3();
			Driver->BaseAddress = Driver->FindBaseAddress();
			Driver->ProcessID = Driver->FindProcessID(xorstr_("r5apex.exe"));
		}
		Sleep(500);
	}
}

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT nCmdShow)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, E("CONOUT$"), E("w"), stdout);
	
	std::string TitleV = uniqueName();
	SetConsoleTitleA(TitleV.c_str());
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
	
	// std::cout << E(" Press (F2) To load HWID Spoofer\n");
	// for (int i = 0; i < 5; ++i) {
	// 	Sleep(1000);
	// 	if (GetAsyncKeyState(VK_F2) & 1) {
	// 		std::cout << E(" HWID Spoofer Disabled at this time...\n");
	// 		break;
	// 	}
	// 	std::cout << " ...\n";
	// }
	INT32 curPiD = GetCurrentProcessId();
	BOOL DriverHandle = Driver->Init();
	if (!DriverHandle) {
		Bypass::LoadGhidraDriver("C:\\Windows\\System32\\Driver.sys");
		DriverHandle = Driver->Init();
	}
	
	// DeletePartition("X");
	Driver->HideProcess(curPiD);
	std::cout << " Press F5 Once in game to inject...\n";
	while (true) {
		Sleep(100);
		if (GetAsyncKeyState(VK_F5) & 1)
			break;

		else if (GetAsyncKeyState(VK_END) & 1) {
			MessageBoxA(NULL, "Process will be closed...", "", MB_OK | MB_ICONINFORMATION);
			exit(1);
			return 1;
		}
	}
	
	Driver->ProcessID = Driver->FindProcessID(xorstr_("r5apex.exe"));
	if (Driver->ProcessID == NULL) {
		std::cout << E(" Process Not Found...\n");
		return 1;
		exit(1);
	}
	
	Driver->CacheCR3();
	uintptr_t BaseAddress = Driver->FindBaseAddress();
	Driver->BaseAddress = BaseAddress;
	if (BaseAddress == NULL) {
		std::cout << E(" BaseAddress Not Found...\n");
		return 1;
		exit(1);
	}

	Beep(723, 200);
	Sleep(500);

	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);

	//CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(CR3Thread), nullptr, NULL, nullptr);
	Overlay::Run(hInstance, nCmdShow, drawLoop);
	return 0;
}