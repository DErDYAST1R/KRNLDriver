#pragma once
#include <ntifs.h>
#include <windef.h>
#include "../driver methods/includes/structs.h"
#include "../driver methods/Helper/Helper.h"


namespace EacBypass
{
	NTSTATUS DecryptCr3Packet(DTBStruct ControlRegister);
}