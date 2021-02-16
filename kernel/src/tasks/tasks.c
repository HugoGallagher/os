#include "tasks/tasks.h"

#include "lib/core.h"
#include "lib/mem.h"

#include "mem/page_allocation.h"
#include "mem/paging.h"

#include "tasks/tss.h"

void tm_init(uint16_t c)
{
    task_manager.k_page_dir = 0xFFFFF000;

    task_manager.tss = kmalloc(sizeof(TSS));
    bzero(task_manager.tss, sizeof(TSS));

    task_manager.tss->ss0 = 0x10;
    task_manager.tss->cs = 0x0B;
    task_manager.tss->ss = 0x13;
    task_manager.tss->ds = 0x13;
    task_manager.tss->es = 0x13;
    task_manager.tss->fs = 0x13;
    task_manager.tss->gs = 0x13;

    task_manager.tss->cr3 = pg_get_phys_addr(task_manager.k_page_dir);

    task_manager.count = c;
    task_manager.tasks = kmalloc(sizeof(Task) * c);

    bzero(task_manager.task_bitmaps, 32*4);

    ta_init(c);
}

void tm_enter_task(FAT32FS* fs, uint16_t id)
{
    memcpy(&(task_manager.tasks[id].page_dir->entries[768]), &(task_manager.k_page_dir->entries[768]), sizeof(PageTableEntry) * (1024 - 768));

    task_manager.tss->esp0 = 1022 * (4*1024*1024) + id * 4096 + 4092;
    task_manager.tss->cr3 = pg_get_phys_addr(task_manager.tasks[id].page_dir);

    pg_load_pd(task_manager.tasks[id].page_dir);
    task_manager.tasks[id].registers.esp_temp = get_esp();
    enter_usr(task_manager.tasks[id].registers);
}
uint16_t tm_create_task(FAT32FS* fs, char* path, uint32_t path_size)
{
    uint16_t c_aligned = task_manager.count + 31;
    c_aligned &= ~0b11111;

    uint16_t id;
    bool slot_found = false;
    for (uint32_t i = 0; i < c_aligned; i++)
    {
        if (~(task_manager.task_bitmaps[i]))
        {
            for (uint32_t j = 0; j < 32; j++)
            {
                if (!(task_manager.task_bitmaps[i] & (1 << j)))
                {
                    id = i*32+j;
                    task_manager.task_bitmaps[i] |= 1 << j;
                    slot_found = true;
                }
                if (slot_found)
                    break;
            }
        }
        if (slot_found)
            break;
    }
    if (!slot_found)
    {
        terminal_writestring("All tasks slots are full\n");
        return;
    }

    tsk_init(task_manager.tasks + id, id, fs, "programs/one/program.bin", 24);

    return id;
}

void tm_syscall(uint32_t index)
{
    //syscalls[index]();
}

TSS* tm_get_tss()
{
    return task_manager.tss;
}

void tsk_init(Task* t, uint16_t id, FAT32FS* fs, char* path, uint32_t path_size)
{
    // copy the kernel page directory
    t->page_dir = ta_alloc_pd();
    memcpy(&(t->page_dir->entries[768]), &(task_manager.k_page_dir->entries[768]), sizeof(PageTableEntry) * (1024 - 768));

    t->pcid = id;

    t->registers.eax = 0;
    t->registers.ecx = 0;
    t->registers.edx = 0;
    t->registers.ebx = 0;
    t->registers.ebp = 0;
    t->registers.esp_temp = 0;
    t->registers.esi = 0;
    t->registers.edi = 0;
    t->registers.flags = get_flags();
    t->registers.eip = 0;
    t->registers.esp = 0xBFFFEFFB;

    // find a 4kb block and assign it as the kernel stack for the task;
    uint32_t kernel_stack = pa_alloc();
    PageTableEntry* ks_pte = pg_get_pte(1022, id);
    ks_pte->data = kernel_stack + (1 << 0) + (1 << 1);

    FAT32DirEntry file_info = fat32_get_file_info(fs, path, path_size);

    uint32_t s;
    s = file_info.size;

    uint32_t tables_needed = s - 1;
    tables_needed += (1024*1024) - (tables_needed % (1024*1024));
    tables_needed /= (1024*1024);

    // iterate over all of the full page tables
    for (uint32_t i = 0; i < (tables_needed - 1); i++)
    {
        PageTable* p_pt = ta_alloc();
        t->page_dir->entries[i].data = pg_get_phys_addr(p_pt) + (1 << 0 | 1 << 1 | 1 << 2);

        for (uint32_t j = 0; j < 1024; j++)
        {
            p_pt->entries[j].data = pa_alloc() | (1 << 0 | 1 << 1 | 1 << 2);
        }
    }
    // now partially allocate last page table
    if (tables_needed > 0)
    {
        PageTable* p_pt = ta_alloc();
        t->page_dir->entries[tables_needed - 1].data = pg_get_phys_addr(p_pt) + (1 << 0 | 1 << 1 | 1 << 2);

        // if s is 1024-aligned
        uint32_t last_entries = s % 1024;
        if (!s)
            last_entries = 1024;

        for (uint32_t j = 0; j < last_entries; j++)
        {
            p_pt->entries[tables_needed - 1].data = pa_alloc() | (1 << 0 | 1 << 1 | 1 << 2);
        }
    }
    // map pt 767 as the tasks stack
    PageTable* ts_pt = ta_alloc();
    t->page_dir->entries[767].data = pg_get_phys_addr(ts_pt) + (1 << 0 | 1 << 1 | 1 << 2);
    for (uint16_t i = 0; i < 1024; i++)
    {
        ts_pt->entries[i].data = pa_alloc() | (1 << 0 | 1 << 1 | 1 << 2);
    }

    // if this is callable while multitasking a lock should be used
    pg_load_pd(t->page_dir->entries);

    // load the program to 0x0
    fat32_read(fs, 0x0, path, path_size);

    pg_load_pd(task_manager.k_page_dir->entries);

}

void ta_init(uint32_t c)
{
    uint32_t c_b2 = c - 1;
    c_b2 += (32*32*32) - (c_b2 % (32*32*32));
    uint32_t bitmaps_needed = c_b2 / (32*32*32);
    uint32_t padded = c_b2 - c;
    task_manager.task_allocs.b2 = kmalloc(sizeof(Bitmap2) * bitmaps_needed);
    bzero(task_manager.task_allocs.b2, sizeof(Bitmap2) * bitmaps_needed);

    // allign addresses to 4kb
    task_manager.task_allocs.pts = kmalloc(sizeof(PageTable) * (c + 1));
    bzero(task_manager.task_allocs.pts, sizeof(PageTable) * (c + 1));
    task_manager.task_allocs.pts += 1;
    task_manager.task_allocs.pts = (uint32_t)task_manager.task_allocs.pts & 0xFFFFF000;

    task_manager.task_allocs.pd_bc = c - 1;
    task_manager.task_allocs.pd_bc += 32 - (task_manager.task_allocs.pd_bc % 32);
    task_manager.task_allocs.pd_bc /= 32;
    task_manager.task_allocs.pd_b = kmalloc(sizeof(uint32_t) * task_manager.task_allocs.pd_bc);
    bzero(task_manager.task_allocs.pd_b, sizeof(uint32_t) * task_manager.task_allocs.pd_bc);

    // allign addresses to 4kb
    task_manager.task_allocs.pds = kmalloc(sizeof(PageDirectory) * (c + 1));
    task_manager.task_allocs.pds += 1;
    task_manager.task_allocs.pds = (uint32_t)task_manager.task_allocs.pds & 0xFFFFF000;

    // set padded pd allocs to allocated
    for (uint32_t i = 0; i < task_manager.task_allocs.pd_bc; i++)
    {
        for (uint32_t j = 0; j < 32; j++)
        {
            if (i * 32 + j > padded)
                task_manager.task_allocs.pd_b[i] |= 1 << j;
        }
    }

    task_manager.task_allocs.a = 0xFFFFFFFF;
    task_manager.task_allocs.bc = bitmaps_needed;

    for (uint32_t i = 0; i < bitmaps_needed; i++)
    {
        task_manager.task_allocs.a ^= 1 << i;

        for (uint32_t j = 0; j < 32; j++)
        {
            for (uint32_t k = 0; k < 32; k++)
            {
                uint32_t index = (k + j * 32 + i * 32*32);

                if (index > c)
                    task_manager.task_allocs.b2[i].b1[j] |= 1 << k;

                if (task_manager.task_allocs.b2[i].b1[j] == 0xFFFFFFFF)
                {
                    task_manager.task_allocs.b2[i].a |= 1 << j;

                    if (task_manager.task_allocs.b2[i].a == 0xFFFFFFFF)
                    {
                        task_manager.task_allocs.a |= 1 << i;
                    }
                }
            }
        }
    }
}
PageTable* ta_alloc()
{
    for (uint32_t i = 0; i < task_manager.task_allocs.bc; i++)
    {
        for (uint32_t j = 0; j < 32; j++)
        {
            for (uint32_t k = 0; k < 32; k++)
            {
                if (!(task_manager.task_allocs.b2[i].b1[j] & 1 << k))
                {
                    uint32_t index = (k + j * 32 + i * 32*32);

                    task_manager.task_allocs.b2[i].b1[j] |= 1 << k;
                    if (task_manager.task_allocs.b2[i].b1[j] == 0xFFFFFFFF)
                    {
                        task_manager.task_allocs.b2[i].a |= 1 << j;

                        if (task_manager.task_allocs.b2[i].a == 0xFFFFFFFF)
                        {
                            task_manager.task_allocs.a |= 1 << i;
                        }
                    }

                    return &(task_manager.task_allocs.pts[index]);
                }
            }
        }
    }
}
void ta_free(uint32_t index)
{
    uint32_t i = (index & 0b111110000000000) >> 10;
    uint32_t j = (index & 0b000001111100000) >> 5;
    uint32_t k = (index & 0b000000000011111) >> 0;

    task_manager.task_allocs.b2[i].b1[j] &= ~(1 << k);

    if (task_manager.task_allocs.b2[i].b1[j] == 0x00000000)
    {
        task_manager.task_allocs.b2[i].a &= ~(1 << k);

        if (task_manager.task_allocs.b2[i].a == 0x00000000)
        {
            if (task_manager.task_allocs.b2[i].a == 0x00000000)
            {
                task_manager.task_allocs.a &= ~(1 << i);
            }
        }
    }
}

PageDirectory* ta_alloc_pd()
{
    for (uint32_t i = 0; i < task_manager.task_allocs.pd_bc; i++)
    {
        if (task_manager.task_allocs.pd_b[i] != 0xFFFFFFFF)
        {
            for (uint32_t j = 0; j < 32; j++)
            {
                if (!(task_manager.task_allocs.pd_b[i] & 1 << j))
                {
                    task_manager.task_allocs.pd_b[i] |= 1 << j;
                    return &(task_manager.task_allocs.pds[i*32+j]);
                }
            }
        }
    }
}
void ta_free_pd(uint32_t index)
{
    uint32_t i_pd = index >> 5;
    index &= 0b11111;

    if (task_manager.task_allocs.pd_b[i_pd] & 1 << index)
        return;

    task_manager.task_allocs.pd_b[i_pd] ^= 1 << index;
}
