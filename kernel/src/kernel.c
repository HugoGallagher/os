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
    //kernel_loop();

    kernel_end = &_kernel_end;

    terminal_initialize();

    terminal_writehex(kernel_end);

    heap_init(&kernel_heap, kernel_end, 1*1024*1024);

    void* p_gdt = kmalloc(64);
    void* p_idt = kmalloc(2048);

    GDTHeader gdt_h;
    gdt_init(&gdt_h, p_gdt);
    gdt_load(gdt_h);
    gdt_reload_cs();

    IDTHeader idt_h;
    idt_h.size = 2047;
    idt_h.descriptors =  p_idt; //kmalloc(2048);
    //terminal_writehex(idt_h.descriptors);

    idt_fill_256(&idt_h);
    idt_load(&idt_h);

    pic_remap();

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
