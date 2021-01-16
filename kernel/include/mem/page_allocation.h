#pragma once

#include "lib/linkedlist.h"
#include "lib/mem.h"
#include "mem/paging.h"

#define AVAILABLE_MEMORY (1024*1024*1024) // only works for systems with 1GB memory
#define PAGE_SIZE (4*1024)
#define PAGE_COUNT (1024*1024)

struct PageDescriptor;
struct PageAllocater;

typedef struct PageDescriptor PageDescriptor;
typedef struct PageAllocater PageAllocater;

struct PageDescriptor
{
    uint16_t index_pd; // index of relevant page table
    uint16_t index_pt; // index of relevent page table entry
} __attribute__((padded));
struct PageAllocater
{
    PageDirEntry* page_directory;
    PageTable* page_tables;

    LinkedList2 free;
};

void pageallocater_init(PageAllocater* pt);
void* pageallocater_alloc(PageAllocater* pt);
void pageallocater_free(PageAllocater* pt, void* addr);
