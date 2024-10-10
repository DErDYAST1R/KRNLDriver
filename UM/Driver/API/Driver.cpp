#include "Driver.h"

uintptr_t	_Driver::BaseAddress;
HANDLE		_Driver::DriverHandle;
int			_Driver::ProcessID;

void GetCpuID(int cpuInfo[4]) {
	__cpuid(cpuInfo, 0);
}

unsigned int CombineCpuInfo(int cpuInfo[4])
{
	return (cpuInfo[1] ^ cpuInfo[2] ^ cpuInfo[3] ^ cpuInfo[0]);
}

void IntegerToString(int value, char* buffer, size_t bufferSize)
{
	snprintf(buffer, bufferSize, "%d", value);
}

bool _Driver::Init()
{
	int cpuInfo[4] = { 0 };
	GetCpuID(cpuInfo);
	unsigned int pcpuInfo = (cpuInfo[1] ^ cpuInfo[2] ^ cpuInfo[3] ^ cpuInfo[0]);
	char buffer[16];
	IntegerToString(pcpuInfo, buffer, sizeof(buffer));


	DriverHandle = CreateFileA(E("\\\\.\\\{068f6114-2b67-4886-b404-8051049b9e66}"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
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
	PROCESSENTRY32 ProcessEntry;
	HANDLE Handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(Handle, &ProcessEntry))
	{
		do
		{
			if (!LI_FN(lstrcmpiA)(ProcessEntry.szExeFile, ProcessName.c_str()))
			{
				CloseHandle(Handle);
				Handle = nullptr;
				return ProcessEntry.th32ProcessID;
			}
		} while (Process32Next(Handle, &ProcessEntry));
	}

	CloseHandle(Handle);
	Handle = nullptr;
	return 0;
}

void _Driver::ReadProcessMemory(PVOID Destination, PVOID Buffer, DWORD Size)
{
	SReadWrite Arguments{ NULL };

	Arguments.ProcessID = ProcessID;
	Arguments.Address	= (uintptr_t)Destination;
	Arguments.Buffer	= (uintptr_t)Buffer;
	Arguments.Size		= Size;

	DeviceIoControl(DriverHandle, CodeRead, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void _Driver::WriteProcessMemory(PVOID Destination, PVOID Buffer, DWORD Size)
{
	SReadWrite Arguments{ NULL };

	Arguments.ProcessID = ProcessID;
	Arguments.Address	= (uintptr_t)Destination;
	Arguments.Buffer	= (uintptr_t)Buffer;
	Arguments.Size		= Size;

	DeviceIoControl(DriverHandle, CodeWrite, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void _Driver::ReadProcessArray(PVOID Destination, PVOID Buffer, DWORD Size)
{
	_Driver::ReadProcessMemory(Destination, Buffer, Size);
}

void _Driver::CacheCR3()
{
	SCR3 Arguments{ NULL };

	Arguments.ProcessID = ProcessID;

	DeviceIoControl(DriverHandle, CodeCR3, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

uintptr_t _Driver::FindBaseAddress()
{
	SBaseAddress Arguments{ NULL };
	uintptr_t Buffer{ NULL };

	Arguments.ProcessID = ProcessID;
	Arguments.Address	= (uintptr_t*)&Buffer;

	DeviceIoControl(DriverHandle, CodeBaseAddress, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);

	return Buffer;
}

uintptr_t _Driver::FindModuleAddress(const char* name)
{
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
	SElevateProcess Arguments{ NULL };

	Arguments.ProcessID = PID;

	DeviceIoControl(DriverHandle, CodeElevateProcess, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void _Driver::MoveMouse(int X, int Y)
{
	SMoveMouse Arguments{ NULL };

	Arguments.X = (LONG)X;
	Arguments.Y = (LONG)Y;

	DeviceIoControl(DriverHandle, CodeMoveMouse, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}

void _Driver::HideProcess(int PID) {
	hidefile Arguments{ NULL };
	Arguments.process_id = PID;
	DeviceIoControl(DriverHandle, CodeHideProcess, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
}