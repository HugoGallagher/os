#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/core.h"
#include "lib/mem.h"
#include "mem/paging.h"

enum page_flags
{
    pf_allocated,
    pf_kernel,
};

void page_set_flag(Page* p, uint32_t pf, bool s)
{
    //p->flags = (p->flags & ~(1 << pf)) | s << pf;
}
uint32_t page_get_flag(Page* p, uint32_t pf)
{
    //return (p->flags >> pf) & ~(1 << 0);
}

void pageallocater_init(PageAllocater* pt, char* start)
{
    pt->all_p = start + 1;
    pt->all_n = start + HEAP_SIZE + sizeof(Page) * PAGE_COUNT;

    bzero(pt->all_p, sizeof(Page) * PAGE_COUNT);
    bzero(pt->all_n, sizeof(LinkedList2Node) * PAGE_COUNT);

    uint32_t kernel_page_count = (uint32_t)((uint32_t)kernel_end / PAGE_SIZE);

    for (uint32_t i = 0; i < (uint32_t)PAGE_COUNT; i++)
    {
        (pt->all_n + i)->data = pt->all_p + i;
        (pt->all_p + i)->addr = i * PAGE_SIZE;

        if (i < kernel_page_count)
        {
            page_set_flag(pt->all_p + i, pf_allocated, true);
            page_set_flag(pt->all_p + i, pf_kernel, true);
        }
        else
        {
            ll2_push_front(&(pt->free), pt->all_n + i);
            page_set_flag(pt->all_p + i, pf_allocated, false);
        }
    }
}

void* pageallocater_alloc(PageAllocater* pt)
{
    if (pt->free.head == pt->free.tail) { return 0; }

    LinkedList2Node* node = ll2_remove_front(&(pt->free));
    Page* page = node->data;
    page->addr = (void*)((page - pt->all_p) * PAGE_SIZE);

    bzero(page->addr, PAGE_SIZE);
    page_set_flag(page, pf_allocated, true);
    page_set_flag(page, pf_kernel, true);

    return page->addr;
}

void pageallocater_free(PageAllocater* pt, void* addr)
{
    if (((uint64_t)addr % PAGE_SIZE) != 0) { return; }

    Page* page = &(pt->all_p[(uint64_t)addr/PAGE_SIZE]);

    page_set_flag(page, pf_allocated, false);
    page_set_flag(page, pf_kernel, false);

    ll2_push_front(&pt->free, &pt->all_n[(uint64_t)addr/PAGE_SIZE]);
}
