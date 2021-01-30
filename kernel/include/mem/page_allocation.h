#pragma once

#include "gnu/multiboot.h"

#include "lib/linkedlist.h"
#include "lib/mem.h"
#include "mem/paging.h"

#define AVAILABLE_MEMORY (1024*1024*1024) // only works for systems with 1GB memory
#define PAGE_SIZE (4*1024)
#define PAGE_COUNT (1024*1024)

struct PABitmap1;
struct PABitmap2;

struct PageAllocater;

typedef struct PABitmap1 PABitmap1;
typedef struct PABitmap2 PABitmap2;

typedef struct PageAllocater PageAllocater;

struct PABitmap1
{
    uint32_t b1[32];
    uint32_t a;
};
struct PABitmap2
{
    PABitmap1 b2[32];
    uint32_t a;
};

struct PageAllocater
{
    PABitmap2* b3;
    uint32_t a;
    uint32_t bitmaps;
};

void pa_init(PageAllocater* pa, multiboot_info_t* mbi);
void* pa_alloc(PageAllocater* pa);
void pa_free(PageAllocater* pa, uint32_t addr);
uint8_t pa_check_addr(uint32_t addr, multiboot_info_t* mbi);
