#include <stddef.h>
#include <stdint.h>

#include "lib/mem.h"
#include "lib/core.h"

#include "interface/terminal.h"

Heap kernel_heap;

void heap_init(Heap* heap, void* start, uint32_t size)
{
    uint32_t alignment = 64;

    size--;
    size += alignment - (size % alignment); // align to 64 bytes

    uint32_t alloc_count = size / alignment;

    heap->start = start;
    heap->max_allocs = alloc_count;
    heap->alloc_data = (uint8_t*)start;

    bzero(heap->alloc_data, sizeof(HeapAllocation) * alloc_count);

    for (int i = 0; i < alloc_count; i++)
    {
        // this maps allocation data to memory, so now only the size needs to be tracked
        heap->alloc_data[i].data = (uint8_t*)start + i * alignment;
    }

    ll1_init_to_addr(&(heap->allocs), (uint8_t*)start + sizeof(HeapAllocation) * alloc_count, alloc_count);
    ll1_push_front(&(heap->allocs), &(heap->alloc_data[alloc_count-1]));

    kmalloc(sizeof(LinkedList1Node) * heap->allocs.node_storage.max_count +
            sizeof(uint64_t) * heap->allocs.node_storage.max_count);
}

// uses kernel_heap
// finds a 64-byte aligned space in heap memory large enough to store the variable
// if there isnt a space large enough it will just return 0
// heap allocations are stored in a linked list, which tracks the starts of allocations
void* kmalloc(uint32_t s)
{
    uint32_t alignment = 64;

    s--;
    s += alignment - (s % alignment); // align

    uint32_t required_allocs = s / alignment;

    LinkedList1Node* current_node = kernel_heap.allocs.head;
    HeapAllocation* current_alloc;
    HeapAllocation* next_alloc;
    uint16_t index = 0;

    bool found_node = false;
    while (current_node != 0)
    {
        if (current_node->next == 0)
        {
            current_alloc = &(kernel_heap.alloc_data[0]);

            current_alloc->size = required_allocs;
            ll1_push_front(&(kernel_heap.allocs), current_alloc);
            found_node = true;

            break;
        }

        current_alloc = (HeapAllocation*)(current_node->data);
        next_alloc = (HeapAllocation*)(current_node->next->data);
        index += current_alloc->size;

        uint8_t* space = next_alloc->data - (current_alloc->data + current_alloc->size * alignment);
        if (space > next_alloc->data)
            space = 0;

        if (space >= s)
        {
            LinkedList1Node* new_node = ll1_insert(&(kernel_heap.allocs), current_node, &(kernel_heap.alloc_data[index]));
            HeapAllocation* new_alloc = (HeapAllocation*)(new_node->data);
            new_alloc->size = required_allocs;
            current_alloc = new_alloc;
            found_node = true;

            break;
        }

        current_node = current_node->next;
    }

    if (found_node)
        return current_alloc->data;
    else
        return 0;
}

void kfree(void* p)
{
    uint32_t alignment = 64;
    if (*(uint32_t*)&p % alignment == 0)
        return;

    LinkedList1Node* current_node = kernel_heap.allocs.head;
    HeapAllocation* current_alloc;
    bool found_node = false;
    for (uint64_t i = 0; i < kernel_heap.allocs.node_storage.count - 1; i++)
    {
        current_alloc = (HeapAllocation*)(current_node->data);

        if (current_alloc->data == p)
        {
            found_node = true;

            ll1_remove(&(kernel_heap.allocs), current_node);
        }

        if (found_node)
        {
            break;
        }
        else
        {
            current_node = current_node->next;
        }
    }
}
