#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "mem/page_allocation.h"

#include "lib/core.h"
#include "lib/mem.h"
#include "mem/paging.h"

void pageallocater_init(PageAllocater* pt)
{
    // pd 

    /*
    ll2_init(pi->free, kmalloc(PAGE_COUNT), PAGE_COUNT);

    uint8_t* pd_address = kmalloc(sizeof(PageDirEntry) * 1024);
    uint8_t* pt_address = kmalloc(sizeof(PageTableEntry) * 1024 * 1024);

    // align to 4kb
    uint8_t* pd_address_aligned = pd_address - 1;
    pd_address_aligned += PAGE_SIZE - (pd_address_aligned % PAGE_SIZE);
    uint8_t* pt_address_aligned = pt_address - 1;
    pt_address_aligned += PAGE_SIZE - (pt_address_aligned % PAGE_SIZE);

    pt->page_directory = pd_address_aligned;
    pt->page_tables = pt_address_aligned;

    bzero(pt->page_directory, sizeof(PageDirEntry) * 1024);
    bzero(pt->page_tables, sizeof(PageTableEntry) * 1024 * 1024);

    uint32_t kernel_page_count = (uint32_t)((uint32_t)kernel_end / PAGE_SIZE);

    PageDirEntry* pde;
    PageTable* table;

    for (uint32_t i = 0; i < (uint32_t)PAGE_COUNT; i++)
    {
        //(pt->all_n + i)->data = pt->all_p + i;
        //(pt->all_p + i)->addr = i * PAGE_SIZE;

        uint32_t i_pd = i - 1;
        i_pd += 1024 - (i_pd % 1024);

        PageDescriptor indexes;
        indexes.index_pd = i_pd / 1024;
        indexes.index_pt = i - i_pd;

        if (i % PAGE_SIZE == 0)
        {
            pde = &(pt->page_directory[indexes.index_pd]);
            pde_set_addr(pde, pt->page_tables[indexes.index_pd]);
            pde_set_flag(pde, pde_present, true);
            // user/supervisor bit should also be set
        }

        table = pde->data & 0xFFFFF000;
        pte_set_addr(&(table->entries[indexes.index_pt]), i * PAGE_SIZE);
        pte_set_flag(&(table->entries[indexes.index_pt]), pte_present, true);

        //PageTable* p_t = pt->page_directory[indexes.index_pd].data & 0xFFFFF000;
        //p_t->entries[indexes.index_pt].data = (i * PAGE_SIZE) & 0xFFFFF000;

        ll2_push_front(&(pt->free), indexes);
    }
    */
}

void* pageallocater_alloc(PageAllocater* pt)
{
    /*
    if (pt->free.head == pt->free.tail) { return 0; }

    LinkedList2Node* node = ll2_remove_front(&(pt->free));
    PageDescriptor indexes = *(PageDescriptor*)&(node->data);

    //page->addr = (void*)((page - pt->all_p) * PAGE_SIZE);

    bzero(page->addr, PAGE_SIZE);
    //page_set_flag(page, pf_allocated, true);
    //page_set_flag(page, pf_kernel, true);

    return page->addr;
    */
}

void pageallocater_free(PageAllocater* pt, void* addr)
{
    /*
    if (((uint64_t)addr % PAGE_SIZE) != 0) { return; }

    Page* page = &(pt->all_p[(uint64_t)addr/PAGE_SIZE]);

    //page_set_flag(page, pf_allocated, false);
    //page_set_flag(page, pf_kernel, false);

    ll2_push_front(&pt->free, &pt->all_n[(uint64_t)addr/PAGE_SIZE]);
    */
}
