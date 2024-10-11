#pragma once
#include <ntifs.h>
#include <windef.h>
#include <wdm.h>
#include "../includes/structs.h"
#include "../includes/ioctl_codes.h"

extern "C" POBJECT_TYPE * IoDriverObjectType;

namespace MouseMovement
{
	NTSTATUS SetCursorPos(MouseMovementStruct x);
	NTSTATUS mousecallback(PMOUSE_OBJECT mouse_obj);
}