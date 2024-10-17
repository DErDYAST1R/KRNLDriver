#pragma once
#include <ntifs.h>
#include <windef.h>
#include "../includes/structs.h"
#include "../includes/ioctl_codes.h"
#include "../../cr3 bypass/cr3.h"
#include "../Helper/Helper.h"
#include <../../driver essentials/import.h>


namespace Read
{
	NTSTATUS ReadMemory(ReadStruct x);
	NTSTATUS read(PVOID target_address, PVOID buffer, SIZE_T size, SIZE_T* bytes_read);
}