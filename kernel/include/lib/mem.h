#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/linkedlist.h"

#define HEAP_SIZE (1024*1024)

struct Heap;

typedef struct Heap Heap;

struct Heap
{
    uint32_t max_allocs; // assumes a minimum level of fragmentation
    uint32_t data_size;

    void* start;

    LinkedList1 allocs;
};

extern Heap kernel_heap;

void heap_init(Heap* heap, void* start, uint32_t size);

void* kmalloc(uint32_t s);
void kfree(void* p);
