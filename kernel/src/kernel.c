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

extern void* pd_boot;
extern void* pt_boot1;
extern void* pt_boot2;

void kmain(multiboot_info_t* mbi)
{
    kernel_end = &_kernel_end;

    terminal_initialize();

    heap_init(&kernel_heap, kernel_end, 6*1024*1024);


    void* p_gdt = kmalloc(64);
    void* p_idt = kmalloc(4096);

    GDTHeader gdt_h;
    gdt_init(&gdt_h, p_gdt);
    gdt_load(gdt_h);
    gdt_reload_cs();
    IDTHeader idt_h;
    idt_h.size = 2047;
    idt_h.descriptors = p_idt;

    idt_fill_256(&idt_h);
    idt_load(&idt_h);

    pic_remap();
    pic_init_pit(0);


    PageAllocater page_allocs;
    pageallocater_init(&page_allocs, mbi);

    terminal_writestring("Works!\n");

    /*
    uint32_t size = 0x20;
    LinkedList1 list;
    ll1_init(&list, size);

    for (uint32_t i = 0; i < size; i++)
    {
        ll1_push_front(&list, i);
    }

    LinkedList1Node* node = list.head;
    for (uint32_t i = 0; i < size; i++)
    {
        if (node->data != (size - (i+1)))
        {
            terminal_writehex(node->data);
            terminal_writehex(i);
        }
        //terminal_writehex(size - (i+1));
        node = node->next;
    }
    */

    kernel_loop();

}

void kernel_loop()
{
    while (true)
    {
        asm("hlt");
    }
}
