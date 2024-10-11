#pragma once
#include <ntifs.h>
#include <windef.h>
#include "../../driver methods/includes/ioctl_codes.h"
#include "../../driver methods/includes/structs.h"
#include "../../driver methods/read/read.h"
#include "../../driver methods/module.address/moduleaddress.h"
#include "../../driver methods/write/write.h"
#include "../../driver methods/base.address/baseaddress.h"
#include "../../driver methods/mouse.movement/mouse.h"
#include "../../driver methods/HideTask/process.h"

namespace Handler
{
	NTSTATUS io_controller(PDEVICE_OBJECT device_obj, PIRP irp);
}