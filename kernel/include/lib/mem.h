#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/linkedlist.h"
#include "mem/paging.h"
#include "mem/page_allocation.h"

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
    uint32_t block_node_count; // nodes are allocated in groups
    uint32_t block_count;
    uint32_t block_size;

    uint32_t size;
    uint32_t max_size;

    PageDirectory* k_page_dir;

    void* start;

    LinkedList1 allocs;

    HeapAllocation* alloc_data;
    uint32_t alloc_data_count;
};

extern Heap kheap;

void kheap_init(void* start);

void kheap_push_block();
void kheap_add_pt();

void* kmalloc(uint32_t s);
void kfree(void* p);
