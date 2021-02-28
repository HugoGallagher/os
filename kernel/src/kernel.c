#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gnu/multiboot.h"

#include "lib/core.h"
#include "lib/mem.h"
#include "interface/terminal.h"
#include "mem/paging.h"
#include "mem/page_allocation.h"
#include "interrupts/gdt.h"
#include "interrupts/idt.h"
#include "interrupts/isrs.h"
#include "interrupts/pic.h"
#include "files/ata.h"
#include "files/fat32.h"

void kmain(multiboot_info_t* mbi)
{
    kernel_end = &_kernel_end;

    terminal_initialize();

    void* heap_start = pa_init(kernel_end, mbi);

    kheap_init(heap_start);

    void* p_gdt = kmalloc(64);
    void* p_idt = kmalloc(65 * 8 - 1);

    tm_init(8);

    GDTHeader gdt_h;
    gdt_init(&gdt_h, p_gdt, tm_get_tss());
    gdt_load(gdt_h);
    gdt_reload_cs();
    tss_load();

    IDTHeader idt_h;
    idt_h.size = (65 * 8 - 1);
    idt_h.descriptors = p_idt;
    pic_remap();
    pic_init_pit(0);
    idt_fill_256(&idt_h);
    idt_load(&idt_h);

    uint16_t identify_data[256];
    uint16_t* mbr;
    MBRPartition partition;

    ata_identify(identify_data);
    mbr = ata_read(0, 0, 1);
    partition = mbr_get_partition(mbr, 0);

    FAT32FS fat32fs;
    fat32_init(&fat32fs, partition);
    tm_set_fs(&fat32fs);

    tm_init_servers(&fat32fs, "sysinfo/servers.txt", 19);

    tm_enter_next_task();

    kernel_loop();
}

void kernel_loop()
{
    while (true)
    {
        asm("hlt");
    }
}
