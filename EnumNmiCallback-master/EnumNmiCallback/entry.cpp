/*++

Copyright (c) 2020-2025, Rog. All rights reserved.

Author:
	Rog

License:
	MIT

--*/
#include "Includes.h"
#include "Utils.hpp"

#define tova(loc) *(PLONG)(loc) + ((loc) + 4)

typedef struct _KNMI_HANDLER_CALLBACK {
	struct _KNMI_HANDLER_CALLBACK* Next;
	PNMI_CALLBACK Callback;
	PVOID Context;
	PVOID Handle;
}KNMI_HANDLER_CALLBACK, * PKNMI_HANDLER_CALLBACK;


//Tested on Windows 10 build 19044
BOOLEAN EnumNmiCallback()
{
	UNICODE_STRING uKeDeregisterNmiCallback = RTL_CONSTANT_STRING(L"KeDeregisterNmiCallback");
	
	ULONG64 KeDeregisterNmiCallbackAddress = (ULONG64)MmGetSystemRoutineAddress(&uKeDeregisterNmiCallback);

	ULONG64 KiDeregisterNmiSxCallback = Utils::FindPattern(KeDeregisterNmiCallbackAddress, 0x20, "48 83 EC 28 E8 ? ? ? ? 48 83 C4 28 C3");
	
	if (!KiDeregisterNmiSxCallback)	{
		return FALSE;
	}
	KiDeregisterNmiSxCallback = tova(KiDeregisterNmiSxCallback + 5);
		
	//DbgPrint("KiDeregisterNmiSxCallback = %llX\n", KiDeregisterNmiSxCallback);

	ULONG64 KiNmiCallbackListHead = Utils::FindPattern(KiDeregisterNmiSxCallback, 0x200, "48 8D 0D ? ? ? ? 0F B6 F8 48 85 DB 0F 84");

	if (!KiNmiCallbackListHead) {
		return FALSE;
	}
	KiNmiCallbackListHead = tova(KiNmiCallbackListHead + 3);
	
	DbgPrint("KiNmiCallbackListHead = %llX\n", KiNmiCallbackListHead);

	for (PKNMI_HANDLER_CALLBACK NmiCallbackInfo = PKNMI_HANDLER_CALLBACK(KiNmiCallbackListHead)->Next; 
		NmiCallbackInfo != NULL; 
		NmiCallbackInfo = NmiCallbackInfo->Next) {
		
		DbgPrint("[NmiCallbackInfo] Enumerating...\n");
		DbgPrint("[NmiCallbackInfo] Callback = %p\n", NmiCallbackInfo->Callback);
		DbgPrint("[NmiCallbackInfo] Context = %p\n", NmiCallbackInfo->Context);
		DbgPrint("[NmiCallbackInfo] Handle = %p\n", NmiCallbackInfo->Handle);
	}

	return TRUE;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status = STATUS_SUCCESS;
	
	DriverObject->DriverUnload = [](PDRIVER_OBJECT DriverObject)->VOID {
		UNREFERENCED_PARAMETER(DriverObject);
	};

	EnumNmiCallback();

	return status;
}
