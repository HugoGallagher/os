#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/linkedlist.h"

struct HeapAllocation;
struct Heap;

typedef struct HeapAllocation HeapAllocation;
typedef struct Heap Heap;

struct HeapAllocation
{
    uint8_t* data;
    uint32_t size;
} __attribute__((packed));

struct Heap
{
    uint32_t max_allocs; // assumes a minimum level of fragmentation
    uint32_t data_size;

    void* start;

    LinkedList1 allocs;
    HeapAllocation* alloc_data;
};

extern Heap kernel_heap;

void heap_init(Heap* heap, void* start, uint32_t size);

void* kmalloc(uint32_t s);
void kfree(void* p);
