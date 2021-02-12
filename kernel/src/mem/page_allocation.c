#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "mem/page_allocation.h"

#include "lib/core.h"
#include "mem/paging.h"
#include "interface/terminal.h"

void* pa_init(void* start, multiboot_info_t* mbi)
{
    // go through every physical page aligned address and check if in use
    // in use = free in mmap and above reserved kernel memory
    // allocations are stored in a three-tiered cascading bitmap

    page_allocs.available_memory = 0;
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
            page_allocs.available_memory += mbim[i].len_low;
    }

    // align downwards to page boundary
    page_allocs.available_memory -= page_allocs.available_memory % PAGE_SIZE;

    //terminal_writehex(page_allocs.available_memory);

    // calculate total needed pages and align (downwards) to 32 to allow for bitmaps
    uint32_t total_pages = page_allocs.available_memory / PAGE_SIZE;
    uint32_t total_pages_aligned = total_pages - 1;
    total_pages_aligned += (32*32*32*32) - (total_pages_aligned % (32*32*32*32));
    uint32_t packed_allocs = total_pages_aligned - total_pages;
    uint32_t bitmaps_needed = (total_pages_aligned / (32*32*32*32)) * 8;
    page_allocs.bitmaps = bitmaps_needed;

    //page_allocs.b3 = kmalloc(sizeof(Bitmap2) * bitmaps_needed);
    page_allocs.b3 = start;
    //bzero(page_allocs.b3, sizeof(Bitmap2) * bitmaps_needed);
    for (uint8_t i = 0; i < (32 - bitmaps_needed); i++)
    {
        page_allocs.a |= 1 << (31 - i);
    }

    for (uint32_t i = 0; i < bitmaps_needed; i++)
    {
        for (uint32_t j = 0; j < 32; j++)
        {
            for (uint32_t k = 0; k < 32; k++)
            {
                for (uint32_t l = 0; l < 32; l++)
                {
                    void* addr = (l + k * 32 + j * 32*32 + i * 32*32*32) * PAGE_SIZE;

                    if (!pa_check_addr(addr, mbi) || addr < 4*1024*1024 * KERNEL_PAGE_TABLES || addr > page_allocs.available_memory)
                    {
                        page_allocs.b3[i].b2[j].b1[k] |= 1 << l;
                        //terminal_writehex(pa->b3[i].b2[j].b1[k]);
                    }
                }
            }
        }
    }

    for (uint32_t i = 0; i < bitmaps_needed; i++)
    {
        for (uint32_t j = 0; j < 32; j++)
        {
            for (uint32_t k = 0; k < 32; k++)
            {
                for (uint32_t l = 0; l < 32; l++)
                {
                    void* addr = (l + k * 32 + j * 32*32 + i * 32*32*32) * PAGE_SIZE;
                    //terminal_writehex(addr);

                    if (!pa_check_addr(addr, mbi) || addr < 4*1024*1024 * KERNEL_PAGE_TABLES || addr > page_allocs.available_memory)
                    {
                        if (page_allocs.b3[i].b2[j].b1[k] == 0xFFFFFFFF)
                        {
                            page_allocs.b3[i].b2[j].a |= 1 << k;

                            if (page_allocs.b3[i].b2[j].a == 0xFFFFFFFF)
                            {
                                page_allocs.b3[i].a |= 1 << j;

                                if (page_allocs.b3[i].a == 0xFFFFFFFF)
                                {
                                    page_allocs.a |= 1 << i;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return start + sizeof(Bitmap2) * bitmaps_needed;
}

uint32_t pa_alloc()
{
    for (uint32_t i = 0; i < page_allocs.bitmaps; i++) { if (!(page_allocs.a & 1 << i))
    {
        for (uint32_t j = 0; j < 32; j++) { if (!(page_allocs.b3[i].a & 1 << j))
        {
            for (uint32_t k = 0; k < 32; k++) { if (!(page_allocs.b3[i].b2[j].a & 1 << k))
            {
                for (uint32_t l = 0; l < 32; l++) { if (!(page_allocs.b3[i].b2[j].b1[k] & 1 << l))
                {
                    page_allocs.b3[i].b2[j].b1[k] |= 1 << l;
                    if (page_allocs.b3[i].b2[j].b1[k] == 0xFFFFFFFF)
                    {
                        page_allocs.b3[i].b2[j].a |= 1 << k;

                        if (page_allocs.b3[i].b2[j].a == 0xFFFFFFFF)
                        {
                            page_allocs.b3[i].a |= 1 << j;

                            if (page_allocs.b3[i].a == 0xFFFFFFFF)
                            {
                                page_allocs.a |= 1 << i;
                            }
                        }
                    }

                    uint32_t addr = (l + k * 32 + j * 32*32 + i * 32*32*32) * PAGE_SIZE;

                    return addr;
                }}
            }}
        }}
    }}
}

void pa_free(uint32_t addr)
{
    uint32_t a = addr / PAGE_SIZE;

    uint32_t i = (a & 0b11111000000000000000) >> 15;
    uint32_t j = (a & 0b00000111110000000000) >> 10;
    uint32_t k = (a & 0b00000000001111100000) >> 5;
    uint32_t l = (a & 0b00000000000000011111) >> 0;

    page_allocs.b3[i].b2[j].b1[k] &= ~(1 << l);

    if (page_allocs.b3[i].b2[j].b1[k] == 0x00000000)
    {
        page_allocs.b3[i].b2[j].a &= ~(1 << k);

        if (page_allocs.b3[i].b2[j].a == 0x00000000)
        {
            page_allocs.b3[i].a &= ~(1 << j);

            if (page_allocs.b3[i].a == 0x00000000)
            {
                page_allocs.a &= ~(1 << i);
            }
        }
    }
}

uint8_t pa_check_addr(uint32_t addr, multiboot_info_t* mbi)
{
    multiboot_memory_map_t* mbim = mbi->mmap_addr;

    for (uint32_t i = 0; i < mbi->mmap_length; i++)
    {
        if ((addr > mbim[i].addr_low && addr < mbim[i].addr_low + mbim[i].len_low) &&
            (addr + PAGE_SIZE > mbim[i].addr_low && addr + PAGE_SIZE < mbim[i].addr_low + mbim[i].len_low))
        {
            return true;
        }
    }

    return false;
}

uint32_t pa_get_available_memory()
{
    return page_allocs.available_memory;
}
