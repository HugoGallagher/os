#pragma once

#include "lib/linkedlist.h"
#include "lib/mem.h"

#define AVAILABLE_MEMORY (1024*1024*1024) // only works for systems with 1GB memory
#define PAGE_SIZE (4*1024)
#define PAGE_COUNT 262144

struct Page;
struct PageAllocater;

typedef struct Page Page;
typedef struct PageAllocater PageAllocater;

struct Page
{
    uint32_t flags;
    void* addr;
};

struct PageAllocater
{
    Page* all_p;
    LLNode* all_n;

    LinkedList free;
};

void page_set_flag(Page* p, uint32_t pf, bool s);
uint32_t page_get_flag(Page* p, uint32_t pf);

void pageallocater_init(PageAllocater* pt, char* start);
void* pageallocater_alloc(PageAllocater* pt);
void pageallocater_free(PageAllocater* pt, void* addr);
