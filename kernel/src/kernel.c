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

typedef void (*program_func)(void);

void kmain(multiboot_info_t* mbi)
{
    kernel_end = &_kernel_end;

    terminal_initialize();

    terminal_writestring("Initialising heap\n");
    heap_init(&kernel_heap, kernel_end, 0x58*1024*1024);
    void* p_gdt = kmalloc(64);
    void* p_idt = kmalloc(2048);

    terminal_writestring("Initialising page frames\n");
    pa_init(mbi);

    terminal_writestring("Initialising GDT\n");
    TaskManager* task_manager = kmalloc(sizeof(TaskManager));
    tm_init(task_manager, 64);

    GDTHeader gdt_h;
    gdt_init(&gdt_h, p_gdt, &task_manager);
    gdt_load(gdt_h);
    gdt_reload_cs();
    tss_reload();

    terminal_writestring("Initialising IDT\n");
    IDTHeader idt_h;
    idt_h.size = 2047;
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

    //tsk_init(task_manager->tasks + 0, task_manager, 512, &fat32fs, "programs/one/program.bin", 24);

//    terminal_write(file, 24);

//    program_func test = file;
//    test();

    terminal_writestring("Works!\n");

    kernel_loop();
}

void kernel_loop()
{
    while (true)
    {
        asm("hlt");
    }
}
