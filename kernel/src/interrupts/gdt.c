#include "interrupts/gdt.h"

#include "interface/terminal.h"

void gdt_init(gdt_header* gh, uint8_t* a)
{
    gh->addr = a;
    gh->size = 0;

    gdt_descriptor null_d;
    null_d.base = 0;
    null_d.limit = 0;
    null_d.type = 0;

    gdt_descriptor code_d;
    code_d.base = 0;
    code_d.limit = 0xFFFFFFFF;
    code_d.type = 0x9A;

    gdt_descriptor data_d;
    data_d.base = 0;
    data_d.limit = 0xFFFFFFFF;
    data_d.type = 0x92;

    gdt_add_entry(gh, null_d);
    gdt_add_entry(gh, code_d);
    gdt_add_entry(gh, data_d);
}

void gdt_add_entry(gdt_header* gh, gdt_descriptor d)
{
    uint8_t* target = gh->addr + gh->size;
    gh->size += 8;

    // from osdev.org

    // Check the limit to make sure that it can be encoded
    if ((d.limit > 65536) && ((d.limit & 0xFFF) != 0xFFF)) {
        //kerror("You can't do that!");
    }
    if (d.limit > 65536) {
        // Adjust granularity if required
        d.limit = d.limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }
    // Encode the limit
    target[0] = d.limit & 0xFF;
    target[1] = (d.limit >> 8) & 0xFF;
    target[6] |= (d.limit >> 16) & 0xF;
    // Encode the base
    target[2] = d.base & 0xFF;
    target[3] = (d.base >> 8) & 0xFF;
    target[4] = (d.base >> 16) & 0xFF;
    target[7] = (d.base >> 24) & 0xFF;
    // And... Type
    target[5] = d.type;
}
