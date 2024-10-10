#include "Helper.h"

#define PAGE_OFFSET_SIZE 12
static const UINT64 PMASK = (~0xfull << 8) & 0xfffffffffull;

UINT64 Helper::translate_linear(UINT64 directoryTableBase, UINT64 virtualAddress) {
	SPOOF_FUNC;
	directoryTableBase &= ~0xf;

	UINT64 pageOffset = virtualAddress & ~(~0ul << PAGE_OFFSET_SIZE);
	UINT64 pte = ((virtualAddress >> 12) & (0x1ffll));
	UINT64 pt = ((virtualAddress >> 21) & (0x1ffll));
	UINT64 pd = ((virtualAddress >> 30) & (0x1ffll));
	UINT64 pdp = ((virtualAddress >> 39) & (0x1ffll));

	SIZE_T readsize = 0;
	UINT64 pdpe = 0;
	Read::read(PVOID(directoryTableBase + 8 * pdp), &pdpe, sizeof(pdpe), &readsize);
	if (~pdpe & 1)
		return 0;

	UINT64 pde = 0;
	Read::read(PVOID((pdpe & PMASK) + 8 * pd), &pde, sizeof(pde), &readsize);
	if (~pde & 1)
		return 0;

	/* 1GB large page, use pde's 12-34 bits */
	if (pde & 0x80)
		return (pde & (~0ull << 42 >> 12)) + (virtualAddress & ~(~0ull << 30));

	UINT64 pteAddr = 0;
	Read::read(PVOID((pde & PMASK) + 8 * pt), &pteAddr, sizeof(pteAddr), &readsize);
	if (~pteAddr & 1)
		return 0;

	/* 2MB large page */
	if (pteAddr & 0x80)
		return (pteAddr & PMASK) + (virtualAddress & ~(~0ull << 21));

	virtualAddress = 0;
	Read::read(PVOID((pteAddr & PMASK) + 8 * pte), &virtualAddress, sizeof(virtualAddress), &readsize);
	virtualAddress &= PMASK;

	if (!virtualAddress)
		return 0;

	return virtualAddress + pageOffset;
}

ULONG64 Helper::find_min(INT32 g, SIZE_T f) {
	SPOOF_FUNC;
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

	INT32 h = (INT32)f;
	ULONG64 result = 0;

	result = (((g) < (h)) ? (g) : (h));

	return result;
}