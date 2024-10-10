#include "Bypass.h"

PWCHAR ToPWCHAR(const std::string& String) 
{
    int BufferSize = MultiByteToWideChar(CP_UTF8, 0, String.c_str(), -1, nullptr, 0);
    if (BufferSize == 0)
    {
        return nullptr;
    }

    PWCHAR Buffer = new WCHAR[BufferSize];
    if (!MultiByteToWideChar(CP_UTF8, 0, String.c_str(), -1, Buffer, BufferSize))
    {
        delete[] Buffer;
        return nullptr;
    }

    return Buffer;
}

NTSTATUS Bypass::LoadGhidraDriver(const char* DriverPath)
{
    PWCHAR GDrvPath = ToPWCHAR(("C:\\Windows\\System32\\Drivers\\gdrv.sys"));
    PWCHAR WideDriverPath = ToPWCHAR(DriverPath);
    return WindLoadDriver(GDrvPath, WideDriverPath, FALSE);
}

NTSTATUS Bypass::UnloadGhidraDriver(const char* DriverPath)
{
    PWCHAR WideDriverPath = ToPWCHAR(DriverPath);
    return WindUnloadDriver(WideDriverPath, FALSE);
}
