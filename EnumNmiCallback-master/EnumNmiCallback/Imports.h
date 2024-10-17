/*++

Copyright (c) 2020-2025, Rog. All rights reserved.

Author:
	Rog

License:
	MIT

--*/
#pragma once


EXTERN_C_START

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
    IN ULONG64 SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetSystemInformation(
    IN ULONG64 SystemInformationClass,
    IN PVOID SystemInformation,
    IN ULONG SystemInformationLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess(
    IN  HANDLE ProcessHandle,
    IN  PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN  ULONG ProcessInformationLength,
    IN  PULONG ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationThread(
    IN HANDLE ThreadHandle,
    IN THREADINFOCLASS ThreadInformationClass,
    OUT PVOID ThreadInformation,
    IN ULONG ThreadInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVirtualMemory(
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress,
    IN MEMORY_INFORMATION_CLASS MemoryInformationClass,
    OUT PVOID MemoryInformation,
    IN SIZE_T MemoryInformationLength,
    OUT PSIZE_T ReturnLength
);

NTSTATUS
NTAPI
ZwTerminateThread(
    IN HANDLE ThreadHandle,
    IN NTSTATUS ExitStatus
    );



NTKERNELAPI
NTSTATUS
NTAPI
MmCopyVirtualMemory(
    IN PEPROCESS FromProcess,
    IN PVOID FromAddress,
    IN PEPROCESS ToProcess,
    OUT PVOID ToAddress,
    IN SIZE_T BufferSize,
    IN KPROCESSOR_MODE PreviousMode,
    OUT PSIZE_T NumberOfBytesCopied
    );

NTKERNELAPI
PPEB
NTAPI
PsGetProcessPeb(IN PEPROCESS Process);

NTKERNELAPI
PVOID
NTAPI
PsGetThreadTeb(IN PETHREAD Thread);

NTKERNELAPI
PVOID
NTAPI
PsGetProcessWow64Process(IN PEPROCESS Process);

NTKERNELAPI
PVOID
NTAPI
PsGetCurrentProcessWow64Process();

NTKERNELAPI
BOOLEAN
NTAPI
KeTestAlertThread(IN KPROCESSOR_MODE AlertMode);

NTKERNELAPI
BOOLEAN
NTAPI
PsIsProtectedProcess(IN PEPROCESS Process);

typedef VOID(NTAPI* PKNORMAL_ROUTINE)(
        PVOID NormalContext,
        PVOID SystemArgument1,
        PVOID SystemArgument2
    );

typedef VOID(NTAPI* PKKERNEL_ROUTINE)(
        PRKAPC Apc,
        PKNORMAL_ROUTINE* NormalRoutine,
        PVOID* NormalContext,
        PVOID* SystemArgument1,
        PVOID* SystemArgument2
    );

typedef VOID(NTAPI* PKRUNDOWN_ROUTINE)(PRKAPC Apc);

NTKERNELAPI
BOOLEAN
NTAPI
KeInsertQueueApc(
    PKAPC Apc,
    PVOID SystemArgument1,
    PVOID SystemArgument2,
    KPRIORITY Increment
    );

NTSYSAPI
PIMAGE_NT_HEADERS
NTAPI
RtlImageNtHeader(PVOID Base);

NTSYSAPI
PVOID
NTAPI
RtlImageDirectoryEntryToData(
    PVOID ImageBase,
    BOOLEAN MappedAsImage,
    USHORT DirectoryEntry,
    PULONG Size
    );

NTKERNELAPI
VOID
FASTCALL
ExfUnblockPushLock(
    IN OUT PEX_PUSH_LOCK PushLock,
    IN OUT PVOID WaitBlock
    );

NTKERNELAPI
PCHAR
PsGetProcessImageFileName(
    __in PEPROCESS Process
    );

NTSYSAPI
NTSTATUS
NTAPI
NtTraceControl(
    _In_ ULONG FunctionCode,
    _In_reads_bytes_opt_(InBufferLen) PVOID InBuffer,
    _In_ ULONG InBufferLen,
    _Out_writes_bytes_opt_(OutBufferLen) PVOID OutBuffer,
    _In_ ULONG OutBufferLen,
    _Out_ PULONG ReturnLength);


NTKERNELAPI 
PVOID 
NTAPI 
RtlFindExportedRoutineByName(
    PVOID ImageBase, 
    PCCH RoutineNam);


NTKERNELAPI
PVOID
NTAPI
PsGetCurrentThreadWin32Thread();


NTSYSAPI
NTSTATUS
NTAPI
ZwGetNextThread(
    __in HANDLE ProcessHandle,
    __in HANDLE ThreadHandle,
    __in ACCESS_MASK DesiredAccess,
    __in ULONG HandleAttributes,
    __in ULONG Flags,
    __out PHANDLE NewThreadHandle);

NTKERNELAPI
PVOID
NTAPI
PsGetThreadWin32Thread(
    _In_ PETHREAD Thread
);

NTKERNELAPI
VOID
PsSetThreadWin32Thread(
    IN OUT PETHREAD Thread,
    IN PVOID Win32Thread,
    IN PVOID PrevWin32Thread
);

NTKERNELAPI
VOID
MmUnlockPagedPool(
    IN PVOID Address,
    IN SIZE_T Size
);

NTKERNELAPI
PVOID
PsGetProcessSectionBaseAddress(
    __in PEPROCESS Process
);

EXTERN_C_END