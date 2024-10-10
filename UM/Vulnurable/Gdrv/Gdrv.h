#pragma once
#include "../../Bypass.h"

#pragma comment(lib, "gD.lib")

extern "C" {
	// swind2.cpp
	NTSTATUS
		WindLoadDriver(
			_In_ PWCHAR LoaderName,
			_In_ PWCHAR DriverName,
			_In_ BOOLEAN Hidden
		);

	NTSTATUS
		WindUnloadDriver(
			_In_ PWCHAR DriverName,
			_In_ BOOLEAN Hidden
		);
}