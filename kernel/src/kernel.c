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
    terminal_initialize();

    kernel_end = &__end;

    gdt_header gdt_h;
    gdt_init(&gdt_h, kernel_end);
    gdt_load(gdt_h);
    gdt_enable_protected_mode();

    idt_header idt_h;
    idt_h.size = 2047;
    idt_h.descriptors = kernel_end + gdt_h.size;

    idt_fill_256(&idt_h);
    idt_load(&idt_h);

    pic_remap();

    heap_init(&kernel_heap, 1, kernel_end);

    kernel_loop();
}

void kernel_loop()
{
    while (true)
    {
        asm("hlt");
    }
}
