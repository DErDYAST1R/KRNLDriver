#include "ntos_base.h"



std::uintptr_t NTOS::resolve_relative_address(uintptr_t instruction, ULONG offset_offset, ULONG instruction_size)
{
    auto instr = instruction;

    const auto rip_offset = *(PLONG)(instr + offset_offset);

    const auto resolved_addr = instr + instruction_size + rip_offset;

    return resolved_addr;
}


std::uintptr_t NTOS::get_ntos_base_address()
{
    typedef unsigned char uint8_t;
    auto Idt_base = reinterpret_cast<uintptr_t>(KeGetPcr()->IdtBase);
    auto align_page = *reinterpret_cast<uintptr_t*>(Idt_base + 4) >> 0xc << 0xc;

    for (; align_page; align_page -= PAGE_SIZE)
    {
        for (int index = 0; index < PAGE_SIZE - 0x7; index++)
        {
            auto current_address = static_cast<intptr_t>(align_page) + index;

            if (*reinterpret_cast<uint8_t*>(current_address) == 0x48
                && *reinterpret_cast<uint8_t*>(current_address + 1) == 0x8D
                && *reinterpret_cast<uint8_t*>(current_address + 2) == 0x1D
                && *reinterpret_cast<uint8_t*>(current_address + 6) == 0xFF)
            {
                // rva
                auto Ntosbase = resolve_relative_address(current_address, 3, 7);
                if (!((UINT64)Ntosbase & 0xfff))
                {
                    return Ntosbase;
                }
            }
        }
    }
    return 0;
}