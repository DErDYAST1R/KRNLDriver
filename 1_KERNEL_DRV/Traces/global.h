#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <ntimage.h>
#include <ntstrsafe.h>
#include <minwindef.h>
#include <psapi.h>

PVOID
GetKernelModuleBase(
	CHAR* ModuleName
);

PVOID FindPatternImage(
	PCHAR Base,
	PCHAR Pattern,
	PCHAR Mask
);

extern PVOID ResolveRelativeAddress(
	_In_ PVOID Instruction,
	_In_ ULONG OffsetOffset,
	_In_ ULONG InstructionSize
);

PVOID GetKernelBase2();

ULONGLONG GetExportedFunction(
	CONST ULONGLONG mod,
	CONST CHAR* name
);

PERESOURCE GetPsLoaded();
UCHAR RandomNumber();
ULONG RandomNumberInRange(ULONG min, ULONG max);

BOOL clearCache(UNICODE_STRING DriverName, ULONG timeDateStamp);
BOOL clearHashBucket(UNICODE_STRING DriverName);
BOOL CleanMmu(UNICODE_STRING DriverName);