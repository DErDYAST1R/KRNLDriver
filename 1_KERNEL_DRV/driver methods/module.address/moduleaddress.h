#pragma once
#include <ntifs.h>
#include <windef.h>
#include "../includes/structs.h"
#include "../includes/ioctl_codes.h"

namespace ModuleAddress
{
	NTSTATUS GetModuleAddress(ModuleAddressStruct x);
}