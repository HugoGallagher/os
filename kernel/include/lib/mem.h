#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/linkedlist.h"

#define HEAP_SIZE (1024*1024)

struct HeapAllocation;
struct Heap;

typedef struct HeapAllocation HeapAllocation;
typedef struct Heap Heap;

struct HeapAllocation
{
    uint32_t data; // first 31 bits are for size, last bit is for allocation;
};

struct Heap
{
    uint64_t max_allocations; // assumes a minimum level of fragmentation
    uint64_t data_size;

    uint8_t* data;
    LinkedList1Node* nodes;
    uint16_t nodes_size;

    LinkedList1 spaces;
};

static Heap kernel_heap;

void ha_set_allocated(HeapAllocation* ha, bool a);
bool ha_get_allocated(HeapAllocation* ha);

void ha_set_size(HeapAllocation* ha, uint64_t s);
uint32_t ha_get_size(HeapAllocation* ha);

void heap_init(Heap* heap, uint64_t size, void* start);

void* kmalloc(uint64_t s);
void kfree(void* p);
