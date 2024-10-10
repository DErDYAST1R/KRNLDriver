#pragma once
#include <ntifs.h>
#include <windef.h>
#include "../read/read.h"

namespace Helper
{
	UINT64 translate_linear(UINT64 directoryTableBase, UINT64 virtualAddress);
	ULONG64 find_min(INT32 g, SIZE_T f);

    static inline void custom_Memcpy(const void* dstp, const void* srcp, UINT len)
    {
        ULONG* dst = (ULONG*)dstp;
        ULONG* src = (ULONG*)srcp;
        UINT i, tail;

        for (i = 0; i < (len / sizeof(ULONG)); i++)
            *dst++ = *src++;

        tail = len & (sizeof(ULONG) - 1);
        if (tail) {

            UCHAR* dstb = (UCHAR*)dstp;
            UCHAR* srcb = (UCHAR*)srcp;

            for (i = len - tail; i < len; i++)
                dstb[i] = srcb[i];
        }
    }
}