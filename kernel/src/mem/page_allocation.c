#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "mem/page_allocation.h"

#include "lib/core.h"
#include "lib/mem.h"
#include "mem/paging.h"
#include "interface/terminal.h"

void pageallocater_init(PageAllocater* pa, multiboot_info_t* mbi)
{
    /*

    go through every physical page aligned address and check if in use
    in use = free in mmap and above reserved kernel memory (currently 8mb)
    allocations are stored in a linkedlist-bitmap hybrid to save memory
    (each address has just over 1 byte of metadata, as well as pt entry)

    */

    uint32_t available_memory = 0;
    multiboot_memory_map_t* mbim = mbi->mmap_addr;

    // find the last available section
    uint32_t last_available = 0;
    for (uint32_t i = 0; i < mbi->mmap_length; i++)
    {
        if (mbim[i].type == MULTIBOOT_MEMORY_AVAILABLE)
            last_available = i;
    }

    // find the total memory (including reserved, since these can be filtered out)
    for (uint32_t i = 0; i < last_available; i++)
    {
        if (mbim[i].len_high == 0 && mbim[i].len_low != 0)
            available_memory += mbim[i].len_low;
    }

    // align to page boundary
    available_memory--;
    available_memory += PAGE_SIZE - (available_memory % PAGE_SIZE);

    // calculate total needed pages and align (downwards) to 32 to allow for bitmaps
    uint32_t total_pages = available_memory / PAGE_SIZE;
    total_pages -= total_pages % 32;
    uint32_t ll_allocs = total_pages / 32;

    ll1_init(&(pa->free), ll_allocs);

    pa->page_allocs = kmalloc(sizeof(PageAllocationElement) * ll_allocs);

    // pages are identity mapped to start with, but as they
    // are allocated and freed they will be mapped randomly
    for (uint32_t i = 0; i <= ll_allocs; i++)
    {
        PageAllocationElement element;
        element.allocated = 0;

        for (uint8_t j = 0; j < 32; j++)
        {
            // get the physical address of the page
            element.addresses[j] = (i * 32 + j) * PAGE_SIZE;
        }

        pa->page_allocs[i] = element;
        ll1_push_front(&(pa->free), &(pa->page_allocs[i]));
    }

    // now allocate the reserved memory as pages
    LinkedList1Node* current_node = pa->free.head;

    uint32_t page_index_h = 0; // the linked list element
    uint32_t page_index_l = 0; // individual entries in addresses
    while (current_node != 0)
    {
        for (uint32_t i = 0; i < 32; i++)
        {
            page_index_l = i;

            PageAllocationElement* element = current_node->data;

            // set the bit here
            element->allocated |= 1 << i;

            uint8_t* phys_addr = (page_index_h * 32 + page_index_l) * PAGE_SIZE;

            // check if the address is above the kernel
            if (phys_addr >= 8*1024*1024)
            {
                bool in_available = false;
                // check if the address is in an available area
                // this is quite slow, lots of room for optimisation
                for (uint32_t j = 0; j < mbi->mmap_length; j++)
                {
                    if (phys_addr > mbim[j].addr_low && phys_addr < mbim[j].addr_low + mbim[j].len_low)
                    {
                        in_available = true;
                    }
                }
                if (in_available)
                {
                    // if the address is available, unset the bit with XOR
                    element->allocated ^= 1 << i;
                }
            }
        }

        page_index_h++;
        page_index_l = 0;
        current_node = current_node->next;
    }
}

void* pageallocater_alloc(PageAllocater* pa)
{

}

void pageallocater_free(PageAllocater* pa, void* addr)
{

}
