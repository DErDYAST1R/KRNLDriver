#pragma once
#include <intrin.h>
#include <ntifs.h>
#include <windef.h>
#include <cstdint>


namespace NTOS
{
    std::uintptr_t resolve_relative_address(uintptr_t instruction, ULONG offset_offset, ULONG instruction_size);
    std::uintptr_t get_ntos_base_address();
}