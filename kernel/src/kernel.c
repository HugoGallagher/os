#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "lib/core.h"
#include "lib/mem.h"
#include "interface/terminal.h"
#include "mem/paging.h"
#include "interrupts/gdt.h"
#include "interrupts/idt.h"
#include "interrupts/isrs.h"
#include "interrupts/pic.h"

void kernel_main(void)
{
    kernel_end = &__end;

    terminal_initialize();

    heap_init(&kernel_heap, kernel_end, 1024*1024);
    //kmalloc(16);

    void* gdt_data = kmalloc(32);

    terminal_writehex(gdt_data);
    //terminal_writehex(kernel_end);

    void* test = kmalloc(16);
    kfree(test);

    gdt_header gdt_h;
    gdt_init(&gdt_h, gdt_data);
    gdt_load(gdt_h);
    gdt_enable_protected_mode();

    idt_header idt_h;
    idt_h.size = 2047;
    idt_h.descriptors = kmalloc(2048);

    idt_fill_256(&idt_h);
    idt_load(&idt_h);

    pic_remap();

    kernel_loop();
}

void kernel_loop()
{
    while (true)
    {
        asm("hlt");
    }
}
