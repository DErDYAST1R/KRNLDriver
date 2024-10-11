#pragma once
#pragma once
#include "Includes.h"

#include "Vulnurable/Gdrv/Gdrv.h"

namespace Bypass
{
	NTSTATUS LoadGhidraDriver(const char* DriverPath);
	NTSTATUS UnloadGhidraDriver(const char* DriverPath);
}