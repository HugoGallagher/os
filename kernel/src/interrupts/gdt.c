#include "interrupts/gdt.h"

#include "interface/terminal.h"

void gdt_init(GDTHeader* gh, uint8_t* a)
{
    gh->addr = a;
    gh->size = 0;

    GDTDescriptor null_d;
    gdt_create_descriptor(&null_d, 0, 0, 0, 0, 0, 0, 0, 0);

    GDTDescriptor k_code_d;
    gdt_create_descriptor(&k_code_d, 0, 0xFFFFFFFF, 1, 0, 1, 1, 0, 1);

    GDTDescriptor k_data_d;
    gdt_create_descriptor(&k_data_d, 0, 0xFFFFFFFF, 1, 0, 1, 0, 0, 1);

    GDTDescriptor u_code_d;
    gdt_create_descriptor(&u_code_d, 0, 0xFFFFFFFF, 1, 3, 1, 1, 1, 1);

    GDTDescriptor u_data_d;
    gdt_create_descriptor(&u_data_d, 0, 0xFFFFFFFF, 1, 3, 1, 0, 0, 1);

    gdt_add_entry(gh, null_d);
    gdt_add_entry(gh, k_code_d);
    gdt_add_entry(gh, k_data_d);
    gdt_add_entry(gh, u_code_d);
    gdt_add_entry(gh, u_data_d);
}
void gdt_create_descriptor(GDTDescriptor* gd, uint32_t base, uint32_t limit, uint8_t present, uint8_t privilege, uint8_t type, uint8_t executable, uint8_t dir_conf, uint8_t read_write)
{
    if (limit > 65536)
    {
        limit = limit >> 12;
        gd->limit_high_flags = 0xC0;
    }
    else
    {
        gd->limit_high_flags = 0x40;
    }

    gd->limit_low = limit & 0xFFFF;
    gd->limit_high_flags |= limit >> 16;

    gd->base_low = base & 0xFFFF;
    gd->base_mid = (base >> 16) & 0xFF;
    gd->base_high = base >> 24;

    gd->access = 0;
    gd->access |= present << 7;
    gd->access |= privilege << 5;
    gd->access |= type << 4;
    gd->access |= executable << 3;
    gd->access |= dir_conf << 2;
    gd->access |= read_write << 1;
}

void gdt_add_entry(GDTHeader* gh, GDTDescriptor d)
{
    uint8_t* target = gh->addr + gh->size;
    gh->size += 8;

    GDTDescriptor* p_d = target;
    *p_d = d;
}
