#pragma once

#include "gnu/multiboot.h"

#include "lib/bitmap.h"
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

struct PageAllocater
{
    Bitmap2* b3;
    uint32_t a;
    uint32_t bitmaps;

    PageDirectory* k_page_dir;
};

static PageAllocater page_allocs;

void pa_init(multiboot_info_t* mbi);
uint32_t pa_alloc();
void pa_free(uint32_t addr);

uint8_t pa_check_addr(uint32_t addr, multiboot_info_t* mbi);
