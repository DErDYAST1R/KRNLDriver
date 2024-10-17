/*++

Copyright (c) 2020-2025, Rog. All rights reserved.

Author:
	Rog

License:
	MIT

--*/
#pragma once
#include <ntifs.h>

namespace Utils
{
	PVOID GetModuleBase(
		PCHAR szModuleName);

	ULONG64 GetImageSectionByName(
		ULONG64 imageBase,
		PCHAR sectionName,
		SIZE_T* sizeOut);

	ULONG64 FindPattern(
		ULONG64 base, 
		SIZE_T size, 
		PCHAR pattern,
		PCHAR mask);

	ULONG64 FindPatternImage(
		PCHAR module,
		PCHAR section,
		PCHAR pattern,
		PCHAR mask);

	ULONG64 FindPattern(
		ULONG64 base, 
		SIZE_T size, 
		PCHAR pattern);

	ULONG64 FindPatternImage(
		PCHAR module, 
		PCHAR section, 
		PCHAR pattern);

	VOID Sleep(
		ULONG Milliseconds);
};
