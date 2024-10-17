/*++

Copyright (c) 2020-2025, Rog. All rights reserved.

Author:
	Rog

License:
	MIT

--*/
#include "Utils.hpp"
#include "Imports.h"
#include "Includes.h"


ULONG64 Utils::FindPattern(ULONG64 base, SIZE_T size, PCHAR pattern, PCHAR mask)
{
    const auto patternSize = strlen(mask);

    for (size_t i = 0; i < size - patternSize; i++) {
        for (size_t j = 0; j < patternSize; j++) {
            if (mask[j] != '?' && *reinterpret_cast<PBYTE>(base + i + j) != static_cast<BYTE>(pattern[j]))
                break;

            if (j == patternSize - 1)
                return (ULONG64)base + i;
        }
    }
    return 0;
}

ULONG64 Utils::FindPattern(ULONG64 base, SIZE_T size, PCHAR pattern)
{
    //find pattern utils
    #define InRange(x, a, b) (x >= a && x <= b) 
    #define GetBits(x) (InRange(x, '0', '9') ? (x - '0') : ((x - 'A') + 0xA))
    #define GetByte(x) ((BYTE)(GetBits(x[0]) << 4 | GetBits(x[1])))

    //get module range
    PBYTE ModuleStart = (PBYTE)base;
    PBYTE ModuleEnd = (PBYTE)(ModuleStart + size);

    //scan pattern main
    PBYTE FirstMatch = nullptr;
    const char* CurPatt = pattern;
    for (; ModuleStart < ModuleEnd; ++ModuleStart)
    {
        bool SkipByte = (*CurPatt == '\?');
        if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
            if (!FirstMatch) FirstMatch = ModuleStart;
            SkipByte ? CurPatt += 2 : CurPatt += 3;
            if (CurPatt[-1] == 0) return (ULONG64)FirstMatch;
        }

        else if (FirstMatch) {
            ModuleStart = FirstMatch;
            FirstMatch = nullptr;
            CurPatt = pattern;
        }
    }
    return NULL;
}

typedef struct _SYSTEM_MODULE
{
    ULONG64 Reserved[2];
    PVOID Base;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT Unknown;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    CHAR ImageName[256];
} SYSTEM_MODULE, * PSYSTEM_MODULE;
typedef struct _SYSTEM_MODULE_INFORMATION
{
    ULONG64 ulModuleCount;
    SYSTEM_MODULE Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

PVOID Utils::GetModuleBase(PCHAR szModuleName)
{
    PVOID result = 0;
    ULONG length = 0;

    ZwQuerySystemInformation(0xb, &length, 0, &length); //SystemModuleInformation
    if (!length) return result;

    const unsigned long tag = 'MEM';
    PSYSTEM_MODULE_INFORMATION system_modules = (PSYSTEM_MODULE_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, length, tag);
    if (!system_modules) return result;

    NTSTATUS status = ZwQuerySystemInformation(0xb, system_modules, length, 0);
    if (NT_SUCCESS(status))
    {
        for (size_t i = 0; i < system_modules->ulModuleCount; i++)
        {
            char* fileName = (char*)system_modules->Modules[i].ImageName + system_modules->Modules[i].ModuleNameOffset;
            if (!strcmp(fileName, szModuleName))
            {
                result = system_modules->Modules[i].Base;
                break;
            }
        }
    }
    ExFreePoolWithTag(system_modules, tag);
    return result;
}


ULONG64 Utils::GetImageSectionByName(ULONG64 imageBase, PCHAR sectionName, SIZE_T* sizeOut)
{
    if (reinterpret_cast<PIMAGE_DOS_HEADER>(imageBase)->e_magic != 0x5A4D)
        return 0;

    const auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS64>(
        imageBase + reinterpret_cast<PIMAGE_DOS_HEADER>(imageBase)->e_lfanew);
    const auto sectionCount = ntHeader->FileHeader.NumberOfSections;

    auto sectionHeader = IMAGE_FIRST_SECTION(ntHeader);
    for (size_t i = 0; i < sectionCount; ++i, ++sectionHeader) {
        if (!strcmp(sectionName, reinterpret_cast<const char*>(sectionHeader->Name))) {
            if (sizeOut)
                *sizeOut = sectionHeader->Misc.VirtualSize;
            return imageBase + sectionHeader->VirtualAddress;
        }
    }
    return 0;
}


ULONG64 Utils::FindPatternImage(PCHAR module, PCHAR section, PCHAR pattern, PCHAR mask)
{
    uintptr_t ModuleBase = 0;
    SIZE_T SectionSize = 0;

    ModuleBase = (uintptr_t)GetModuleBase(module);
    if (!ModuleBase)
        return 0;

    const auto SectionBase = GetImageSectionByName(ModuleBase, section, &SectionSize);
    if (!SectionBase)
        return 0;

    return FindPattern(SectionBase, SectionSize, pattern, mask);
}

ULONG64 Utils::FindPatternImage(PCHAR module, PCHAR section, PCHAR pattern)
{
    uintptr_t ModuleBase = 0;
    SIZE_T SectionSize = 0;

    ModuleBase = (uintptr_t)GetModuleBase(module);
    if (!ModuleBase)
        return 0;

    const auto SectionBase = GetImageSectionByName(ModuleBase, section, &SectionSize);
    if (!SectionBase)
        return 0;

    return FindPattern(SectionBase, SectionSize, pattern);
}

VOID Utils::Sleep(ULONG Milliseconds)
{
	LARGE_INTEGER Timeout;
	Timeout.QuadPart = -1 * 10000LL * (LONGLONG)Milliseconds;
	KeDelayExecutionThread(KernelMode, FALSE, &Timeout);
}
