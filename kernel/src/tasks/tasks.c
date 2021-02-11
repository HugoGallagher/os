#include "tasks/tasks.h"

#include "lib/core.h"
#include "lib/mem.h"

#include "mem/page_allocation.h"
#include "mem/paging.h"

#include "tasks/tss.h"

void tm_init(TaskManager* tm, uint16_t c)
{
    tm->k_page_dir = 0xFFFFF000;
    pg_load_cr3(tm->k_page_dir);

    tm->tss = kmalloc(sizeof(TSS));
    bzero(tm->tss, sizeof(TSS));
    //tm->tss->ss0 = 0x10;
    //tm->tss->cr3 = kpd;

    tm->count = c;
    tm->tasks = kmalloc(sizeof(Task) * c);

    ta_init(&(tm->task_allocs), c*32);
}

void tsk_init(Task* t, TaskManager* tm, uint32_t s, FAT32FS* fs, char* path, uint32_t path_size)
{
    t->page_dir = ta_alloc_pd(&(tm->task_allocs));

    // copy the kernel page directory
    memcpy(&(t->page_dir->entries[768]), &(tm->k_page_dir->entries[768]), sizeof(PageTableEntry) * (1024 - 768));

    uint32_t tables_needed = s - 1;
    tables_needed += 1024 - (tables_needed % 1024);
    tables_needed /= 1024;

    // iterate over all of the full page tables
    for (uint32_t i = 0; i < (tables_needed - 1); i++)
    {
        PageTable* p_pt = ta_alloc(&(tm->task_allocs));
        t->page_dir->entries[i].data = pg_get_phys_addr(p_pt) + (1 << 0 | 1 << 1 | 1 << 2);

        for (uint32_t j = 0; j < 1024; j++)
        {
            p_pt->entries[j].data = pa_alloc() | (1 << 0 | 1 << 1 | 1 << 2);
            //terminal_writehex(p_pt->entries[768].data);
        }
    }
    // now partially allocate last page table
    if (tables_needed > 0)
    {
        PageTable* p_pt = ta_alloc(&(tm->task_allocs));
        t->page_dir->entries[tables_needed - 1].data = pg_get_phys_addr(p_pt) + (1 << 0 | 1 << 1 | 1 << 2);

        // if s is 1024-aligned
        uint32_t last_entries = s % 0;
        if (!s)
            last_entries = 1024;

        for (uint32_t j = 0; j < last_entries; j++)
        {
            p_pt->entries[tables_needed - 1].data = pa_alloc() | (1 << 0 | 1 << 1 | 1 << 2);
        }
    }

    // if this is callable while multitasking a lock should be used
    //pg_load_cr3(t->page_dir->entries);

    terminal_writehex(t->page_dir->entries);
    terminal_writehex(kmalloc(512));

    //fat32_read(fs, 0x0, path, path_size);

    //terminal_writehex(fs);

    //pg_load_cr3(tm->k_page_dir->entries);
}

void ta_init(TaskAllocater* ta, uint32_t c)
{
    uint32_t c_b2 = c - 1;
    c_b2 += (32*32*32) - (c_b2 % (32*32*32));
    uint32_t bitmaps_needed = c_b2 / (32*32*32);
    uint32_t padded = c_b2 - c;
    ta->b2 = kmalloc(sizeof(Bitmap2) * bitmaps_needed);
    bzero(ta->b2, sizeof(Bitmap2) * bitmaps_needed);

    // allign addresses to 4kb
    ta->pts = kmalloc(sizeof(PageTable) * (c + 1));
    bzero(ta->pts, sizeof(PageTable) * (c + 1));
    ta->pts += 1;
    ta->pts = (uint32_t)ta->pts & 0xFFFFF000;

    ta->pd_bc = c - 1;
    ta->pd_bc += 32 - (ta->pd_bc % 32);
    ta->pd_bc /= 32;
    ta->pd_b = kmalloc(sizeof(uint32_t) * ta->pd_bc);
    bzero(ta->pd_b, sizeof(uint32_t) * ta->pd_bc);

    // allign addresses to 4kb
    ta->pds = kmalloc(sizeof(PageDirectory) * (c + 1));
    ta->pds += 1;
    ta->pds = (uint32_t)ta->pds & 0xFFFFF000;
    terminal_writehex(ta->pds);
    terminal_writehex(sizeof(PageDirectory) * (c + 1));
    terminal_writehex(kmalloc(512));

    // set padded pd allocs to allocated
    for (uint32_t i = 0; i < ta->pd_bc; i++)
    {
        for (uint32_t j = 0; j < 32; j++)
        {
            if (i * 32 + j > padded)
                ta->pd_b[i] |= 1 << j;
        }
    }

    ta->a = 0xFFFFFFFF;
    ta->bc = bitmaps_needed;

    for (uint32_t i = 0; i < bitmaps_needed; i++)
    {
        ta->a ^= 1 << i;

        for (uint32_t j = 0; j < 32; j++)
        {
            for (uint32_t k = 0; k < 32; k++)
            {
                uint32_t index = (k + j * 32 + i * 32*32);

                if (index > c)
                    ta->b2[i].b1[j] |= 1 << k;

                if (ta->b2[i].b1[j] == 0xFFFFFFFF)
                {
                    ta->b2[i].a |= 1 << j;

                    if (ta->b2[i].a == 0xFFFFFFFF)
                    {
                        ta->a |= 1 << i;
                    }
                }
            }
        }
    }
}
PageTable* ta_alloc(TaskAllocater* ta)
{
    for (uint32_t i = 0; i < ta->bc; i++)
    {
        for (uint32_t j = 0; j < 32; j++)
        {
            for (uint32_t k = 0; k < 32; k++)
            {
                if (!(ta->b2[i].b1[j] & 1 << k))
                {
                    uint32_t index = (k + j * 32 + i * 32*32);

                    ta->b2[i].b1[j] |= 1 << k;
                    if (ta->b2[i].b1[j] == 0xFFFFFFFF)
                    {
                        ta->b2[i].a |= 1 << j;

                        if (ta->b2[i].a == 0xFFFFFFFF)
                        {
                            ta->a |= 1 << i;
                        }
                    }

                    return &(ta->pts[index]);
                }
            }
        }
    }
}
void ta_free(TaskAllocater* ta, uint32_t index)
{
    uint32_t i = (index & 0b111110000000000) >> 10;
    uint32_t j = (index & 0b000001111100000) >> 5;
    uint32_t k = (index & 0b000000000011111) >> 0;

    ta->b2[i].b1[j] &= ~(1 << k);

    if (ta->b2[i].b1[j] == 0x00000000)
    {
        ta->b2[i].a &= ~(1 << k);

        if (ta->b2[i].a == 0x00000000)
        {
            if (ta->b2[i].a == 0x00000000)
            {
                ta->a &= ~(1 << i);
            }
        }
    }
}

PageDirectory* ta_alloc_pd(TaskAllocater* ta)
{
    for (uint32_t i = 0; i < ta->pd_bc; i++)
    {
        if (ta->pd_b[i] != 0xFFFFFFFF)
        {
            for (uint32_t j = 0; j < 32; j++)
            {
                if (!(ta->pd_b[i] & 1 << j))
                {
                    ta->pd_b[i] |= 1 << j;
                    return &(ta->pds[i*32+j]);
                }
            }
        }
    }
}
void ta_free_pd(TaskAllocater* ta, uint32_t index)
{
    uint32_t i_pd = index >> 5;
    index &= 0b11111;

    if (ta->pd_b[i_pd] & 1 << index)
        return;

    ta->pd_b[i_pd] ^= 1 << index;
}
