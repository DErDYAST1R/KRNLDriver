#include "Driver.h"

uintptr_t	_Driver::BaseAddress;
HANDLE		_Driver::DriverHandle;
int			_Driver::ProcessID;
uintptr_t	_Driver::ClientDLL;

bool _Driver::Init()
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

	DriverHandle = CreateFileA(E("\\\\.\\\{564c6c62-ee64-4097-8da1-c17137850a1c}"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (!DriverHandle || DriverHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	return true;
}

bool _Driver::Install(const char* certContent)
{
	DWORD certSize = 0;
	LI_FN(CryptStringToBinaryA)(certContent, LI_FN(strlen)(certContent), CRYPT_STRING_BASE64, nullptr, &certSize, nullptr, nullptr);
	std::vector<BYTE> certBuffer(certSize);
	LI_FN(CryptStringToBinaryA)(certContent, LI_FN(strlen)(certContent), CRYPT_STRING_BASE64, certBuffer.data(), &certSize, nullptr, nullptr);

	PCCERT_CONTEXT pCertContext = LI_FN(CertCreateCertificateContext)(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, certBuffer.data(), certBuffer.size());
	if (!pCertContext) 
	{
		return false;
	}

	HCERTSTORE hCertStore = LI_FN(CertOpenStore)(CERT_STORE_PROV_SYSTEM, 0, NULL, CERT_SYSTEM_STORE_LOCAL_MACHINE, E(L"ROOT"));
	if (!hCertStore) 
	{
		LI_FN(CertFreeCertificateContext)(pCertContext);
		return false;
	}

	if (!LI_FN(CertAddCertificateContextToStore)(hCertStore, pCertContext, CERT_STORE_ADD_REPLACE_EXISTING, nullptr))
	{
		LI_FN(CertCloseStore)(hCertStore, CERT_CLOSE_STORE_FORCE_FLAG);
		LI_FN(CertFreeCertificateContext)(pCertContext);
		return false;
	}

	LI_FN(CertCloseStore)(hCertStore, CERT_CLOSE_STORE_FORCE_FLAG);
	LI_FN(CertFreeCertificateContext)(pCertContext);
	return true;
}

bool _Driver::Load(std::vector<std::uint8_t> Bytes)
{
	return true;
}

int _Driver::FindProcessID(std::string ProcessName)
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

	PROCESSENTRY32 ProcessEntry;
	HANDLE Handle = LI_FN(CreateToolhelp32Snapshot)(TH32CS_SNAPPROCESS, 0);
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

	if (LI_FN(Process32First)(Handle, &ProcessEntry))
	{
		do
		{
			if (!LI_FN(lstrcmpiA)(ProcessEntry.szExeFile, ProcessName.c_str()))
			{
				LI_FN(CloseHandle)(Handle);
				Handle = nullptr;
				return ProcessEntry.th32ProcessID;
			}
		} while (LI_FN(Process32Next)(Handle, &ProcessEntry));
	}

	LI_FN(CloseHandle)(Handle);
	Handle = nullptr;
	return 0;
}

void _Driver::ReadProcessMemory(PVOID Destination, PVOID Buffer, DWORD Size)
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

	SReadWrite Arguments{ NULL };

	Arguments.ProcessID = ProcessID;
	Arguments.Address	= (uintptr_t)Destination;
	Arguments.Buffer	= (uintptr_t)Buffer;
	Arguments.Size		= Size;

	if (Driver->BaseAddress == NULL) {
		return;
	}

	DeviceIoControl(DriverHandle, CodeRead, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void _Driver::WriteProcessMemory(PVOID Destination, PVOID Buffer, DWORD Size)
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

	SReadWrite Arguments{ NULL };

	Arguments.ProcessID = ProcessID;
	Arguments.Address	= (uintptr_t)Destination;
	Arguments.Buffer	= (uintptr_t)Buffer;
	Arguments.Size		= Size;

	if (Driver->BaseAddress == NULL) {
		return;
	}

	DeviceIoControl(DriverHandle, CodeWrite, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void _Driver::ReadProcessArray(PVOID Destination, PVOID Buffer, DWORD Size)
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

	if (Driver->BaseAddress == NULL) {
		return;
	}

	LI_FN(_Driver::ReadProcessMemory)(Destination, Buffer, Size);
}

void _Driver::CacheCR3()
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

	SCR3 Arguments{ NULL };

	Arguments.ProcessID = ProcessID;

	DeviceIoControl(DriverHandle, CodeCR3, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

uintptr_t _Driver::FindBaseAddress()
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

	SBaseAddress Arguments{ NULL };
	uintptr_t Buffer{ NULL };

	Arguments.ProcessID = ProcessID;
	Arguments.Address	= (uintptr_t*)&Buffer;

	DeviceIoControl(DriverHandle, CodeBaseAddress, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);

	return Buffer;
}

uintptr_t _Driver::FindModuleAddress(const char* name)
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

	SModuleAddress Arguments{ NULL };
	uintptr_t Buffer{ NULL };

	Arguments.ProcessID = ProcessID;
	Arguments.ModuleName = name;
	Arguments.Address = (uintptr_t*)&Buffer;

	DeviceIoControl(DriverHandle, CodeModuleAddress, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);

	return Buffer;
}

void _Driver::ElevateProcess(int PID)
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

	SElevateProcess Arguments{ NULL };

	Arguments.ProcessID = PID;

	DeviceIoControl(DriverHandle, CodeElevateProcess, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void _Driver::MoveMouse(int X, int Y)
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

	SMoveMouse Arguments{ NULL };

	Arguments.X = (LONG)X;
	Arguments.Y = (LONG)Y;

	DeviceIoControl(DriverHandle, CodeMoveMouse, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void _Driver::HideProcess(int PID) {
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

	hidefile Arguments{ NULL };
	Arguments.process_id = PID;
	DeviceIoControl(DriverHandle, CodeHideProcess, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}