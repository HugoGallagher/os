#pragma once

#include "gnu/multiboot.h"

#include "lib/linkedlist.h"
#include "lib/mem.h"
#include "mem/paging.h"

#define AVAILABLE_MEMORY (1024*1024*1024) // only works for systems with 1GB memory
#define PAGE_SIZE (4*1024)
#define PAGE_COUNT (1024*1024)

struct PageDescriptor;
struct PageAllocationElement;
struct PageAllocater;

typedef struct PageDescriptor PageDescriptor;
typedef struct PageAllocationElement PageAllocationElement;
typedef struct PageAllocater PageAllocater;

struct PageDescriptor
{
    uint16_t index_pd; // index of relevant page table
    uint16_t index_pt; // index of relevent page table entry
} __attribute__((padded));

struct PageAllocationElement
{
    uint8_t* addresses[32];
    uint32_t allocated;
} __attribute__((packed));
struct PageAllocater
{
    //PageDirEntry* page_directory;
    //PageTable* page_tables;

    PageAllocationElement* page_allocs;

    LinkedList1 free;
};

void pageallocater_init(PageAllocater* pa, multiboot_info_t* mbi);
void* pageallocater_alloc(PageAllocater* pa);
void pageallocater_free(PageAllocater* pa, void* addr);
