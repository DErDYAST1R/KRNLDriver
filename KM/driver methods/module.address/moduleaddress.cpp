#include "moduleaddress.h"

_declspec(noinline) uintptr_t attach_process(uintptr_t process)
{
	auto current_thread = (uintptr_t)udman_spoof(KeGetCurrentThread)();
	if (!current_thread)
		return 0;

	auto apc_state = *(uintptr_t*)(current_thread + 0x98);
	auto old_process = *(uintptr_t*)(apc_state + 0x20);
	*(uintptr_t*)(apc_state + 0x20) = process;

	auto dir_table_base = *(uintptr_t*)(process + 0x28);

	__writecr3(dir_table_base);

	return old_process;
}

typedef struct _PEB_LDR_DATA {
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY ModuleListLoadOrder;
	LIST_ENTRY ModuleListMemoryOrder;
	LIST_ENTRY ModuleListInitOrder;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	BYTE Reserved1[16];
	PVOID Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

typedef void(__stdcall* PPS_POST_PROCESS_INIT_ROUTINE)(void);

typedef struct _PEB {
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[1];
	PVOID Reserved3[2];
	PPEB_LDR_DATA Ldr;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	PVOID Reserved4[3];
	PVOID AtlThunkSListPtr;
	PVOID Reserved5;
	ULONG Reserved6;
	PVOID Reserved7;
	ULONG Reserved8;
	ULONG AtlThunkSListPtr32;
	PVOID Reserved9[45];
	BYTE Reserved10[96];
	PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
	BYTE Reserved11[128];
	PVOID Reserved12[1];
	ULONG SessionId;
} PEB, * PPEB;

extern "C"
{
	NTKERNELAPI PPEB PsGetProcessPeb(IN PEPROCESS Process);
}

NTSTATUS ModuleAddress::GetModuleAddress(ModuleAddressStruct x) {
	SPOOF_FUNC;

	if (!x->process_id)
		return STATUS_UNSUCCESSFUL;

	int AGEHUGAIUHVAR1 = 1613513513;
	int AGEHUGAIUHVAR2 = 1357981351;
	int AGEHUGAIUHVAR3 = 6135413635;
	int AGEHUGAIUHVAR4 = 1351351515;
	int AGEHUGAIUHVAR5 = 6135135151;

	while (AGEHUGAIUHVAR1 == 13651351)
	{
		AGEHUGAIUHVAR1 += 1;
	}

	while (AGEHUGAIUHVAR2 == 3151351351)
	{
		AGEHUGAIUHVAR2 += 1;
	}

	while (AGEHUGAIUHVAR3 == 136511351135351)
	{
		AGEHUGAIUHVAR3 += 1;
	}

	while (AGEHUGAIUHVAR4 == 13613551351135135)
	{
		AGEHUGAIUHVAR4 += 1;
	}

	while (AGEHUGAIUHVAR5 == 13513515115)
	{
		AGEHUGAIUHVAR5 += 1;
	}

	uintptr_t out = 0;
	uintptr_t o_process = 0;

	PEPROCESS process = 0;
	udman_spoof(PsLookupProcessByProcessId)((HANDLE)x->process_id, &process);
	if (!process)
		return STATUS_UNSUCCESSFUL;

	ANSI_STRING ansi_name;
	udman_spoof(RtlInitAnsiString)(&ansi_name, x->moduleName);

	UNICODE_STRING compare_name;
	udman_spoof(RtlAnsiStringToUnicodeString)(&compare_name, &ansi_name, TRUE);

	o_process = attach_process((uintptr_t)process);
	if (!o_process)
		return STATUS_UNSUCCESSFUL;

	PPEB pPeb = udman_spoof(PsGetProcessPeb)(process);

	if (pPeb)
	{
		PPEB_LDR_DATA pLdr = (PPEB_LDR_DATA)pPeb->Ldr;

		if (pLdr)
		{
			for (PLIST_ENTRY listEntry = (PLIST_ENTRY)pLdr->ModuleListLoadOrder.Flink;
				listEntry != &pLdr->ModuleListLoadOrder;
				listEntry = (PLIST_ENTRY)listEntry->Flink) {


				PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(listEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);

				if (udman_spoof(RtlCompareUnicodeString)(&pEntry->BaseDllName, &compare_name, TRUE) == 0)
				{
					out = (uint64_t)pEntry->DllBase;
					break;
				}
			}
		}
	}

	attach_process(o_process);

	udman_spoof(RtlFreeUnicodeString)(&compare_name);

	RtlCopyMemory(x->address, &out, sizeof(out));
	udman_spoof(ObfDereferenceObject)(process);

	return STATUS_SUCCESS;
}
