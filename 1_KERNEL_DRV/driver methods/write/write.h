#pragma once
#include <ntifs.h>
#include <windef.h>
#include "../includes/structs.h"


namespace Write
{
	NTSTATUS WriteMemory(WriteStruct x);
	NTSTATUS write(PVOID target_address, PVOID buffer, SIZE_T size, SIZE_T* bytes_read);
}