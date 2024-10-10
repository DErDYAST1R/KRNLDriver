#pragma once
#include <ntifs.h>
#include <windef.h>
#include "../../driver essentials/imports.h"
#include "callstack.h"
#include <ntddmou.h>

typedef struct _rw {
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} rw, * WriteStruct;

typedef struct _r {
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} rwr, * ReadStruct;

typedef struct _hf {
	INT32 process_id;
} hidefile, * HideFileStruct;

typedef struct _ba {
	INT32 process_id;
	ULONGLONG* address;
} ba, * BaseAddressStruct;

typedef struct _ma {
	INT32 process_id;
	const char* moduleName;
	ULONGLONG* address;
} ma, * ModuleAddressStruct;

typedef struct _cr3 {
	INT32 process_id;
} cr3, * DTBStruct;

typedef struct _ep {
	INT32 process_id;
} ep, * ElevateProcess;

typedef struct _restart {
	bool bResetCr3;
} restart, * RestartStruct;

typedef VOID
(*MouseClassServiceCallback)(
	PDEVICE_OBJECT DeviceObject,
	PMOUSE_INPUT_DATA InputDataStart,
	PMOUSE_INPUT_DATA InputDataEnd,
	PULONG InputDataConsumed
	);

typedef struct _MOUSE_OBJECT
{
	PDEVICE_OBJECT mouse_device;
	MouseClassServiceCallback service_callback;
} MOUSE_OBJECT, * PMOUSE_OBJECT;

typedef struct _movemouse {
	long x;
	long y;
	unsigned short button_flags;
} movemouse, * MouseMovementStruct;

typedef struct _SYSTEM_BIGPOOL_ENTRY
{
	union {
		PVOID VirtualAddress;
		ULONG_PTR NonPaged : 1;
	};
	ULONG_PTR SizeInBytes;
	union {
		UCHAR Tag[4];
		ULONG TagUlong;
	};
} SYSTEM_BIGPOOL_ENTRY, * PSYSTEM_BIGPOOL_ENTRY;

typedef struct _SYSTEM_BIGPOOL_INFORMATION {
	ULONG Count;
	SYSTEM_BIGPOOL_ENTRY AllocatedInfo[ANYSIZE_ARRAY];
} SYSTEM_BIGPOOL_INFORMATION, * PSYSTEM_BIGPOOL_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemModuleInformation = 11,
	SystemBigPoolInformation = 0x42
} SYSTEM_INFORMATION_CLASS;

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	LIST_ENTRY HashLinks;
	PVOID SectionPointer;
	ULONG CheckSum;
	ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

extern "C"
{
	NTSYSAPI NTSTATUS NTAPI ZwSetInformationProcess(__in HANDLE ProcessHandle, __in PROCESSINFOCLASS ProcessInformationClass, __in_bcount(ProcessInformationLength) PVOID ProcessInformation, __in ULONG ProcessInformationLength);
	_Must_inspect_result_ NTSYSAPI NTSTATUS NTAPI ZwAdjustPrivilegesToken(_In_ HANDLE TokenHandle, _In_ BOOLEAN DisableAllPrivileges, _In_opt_ PTOKEN_PRIVILEGES NewState, _In_ ULONG BufferLength, _Out_writes_bytes_to_opt_(BufferLength, *ReturnLength) PTOKEN_PRIVILEGES PreviousState, _When_(PreviousState != NULL, _Out_) PULONG ReturnLength);
}

EXTERN_C NTSTATUS NTAPI MmCopyVirtualMemory
(
	PEPROCESS SourceProcess,
	PVOID SourceAddress,
	PEPROCESS TargetProcess,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);

extern "C"
{
	PLIST_ENTRY NTKERNELAPI PsLoadedModuleList;
	NTKERNELAPI PVOID NTAPI RtlFindExportedRoutineByName(_In_ PVOID ImageBase, _In_ PCCH RoutineName);
	NTSTATUS NTAPI IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitializationFunction);
	PVOID NTAPI PsGetProcessSectionBaseAddress(PEPROCESS Process);
	NTSTATUS NTAPI ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, PULONG returnLength);
	NTSYSAPI NTSTATUS NTAPI ZwQueryInformationProcess(__in HANDLE ProcessHandle, __in PROCESSINFOCLASS ProcessInformationClass, __out_bcount(ProcessInformationLength) PVOID ProcessInformation, __in ULONG ProcessInformationLength, __out_opt PULONG ReturnLength);
	NTSYSAPI NTSTATUS NTAPI ObReferenceObjectByName(_In_ PUNICODE_STRING ObjectName, _In_ ULONG Attributes, _In_opt_ PACCESS_STATE AccessState, _In_opt_ ACCESS_MASK DesiredAccess, _In_ POBJECT_TYPE ObjectType, _In_ KPROCESSOR_MODE AccessMode, _Inout_opt_ PVOID ParseContext, _Out_ PVOID* Object);
};

