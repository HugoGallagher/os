#include <stddef.h>
#include <stdint.h>

#include "lib/mem.h"

#include "interface/terminal.h"

Heap kernel_heap;

void heap_init(Heap* heap, void* start, uint32_t size)
{
    size--;
    size += 16 - (size % 16); // align to 16 bytes

    uint32_t alloc_count = size / 16;
    uint32_t max_allocs = alloc_count / 16;
    // ^ this saves memory by assuming that the heap will never reach a certain point of fragmentation

    heap->start = start;
    heap->max_allocs = max_allocs;

    ll1_init(&(heap->allocs), start, max_allocs);
    ll1_push_front(&(heap->allocs), ((uint8_t*)start) + size);
    ll1_push_front(&(heap->allocs), start);
}

// uses kernel_heap
// finds a 16-byte aligned space in heap memory large enough to store the variable
// if there isnt a space large enough it will just return 0
// heap allocations are stored in a linked list, which tracks the starts of allocations
// the lowest bit of node->data determines if the gap is an allocation (1) or a space (0)
void* kmalloc(uint32_t s)
{
    s--;
    s += 16 - (s % 16); // align to 16 bytes
    uint32_t segments_needed = s / 16;

    LinkedList1Node* current_node = kernel_heap.allocs.head;
    bool found_suitable_node = false;
    for (uint32_t i = 0; i < kernel_heap.allocs.node_storage.count - 1; i++)
    {
        if ((uint32_t)((uintptr_t)current_node->next->data & 0xFFFFFFFE) - ((uintptr_t)current_node->data & 0xFFFFFFFE) >= s)
        {
            found_suitable_node = true;
            LinkedList1Node* new_node = ll1_push_front(&(kernel_heap.allocs), ((uintptr_t)current_node->next->data & 0xFFFFFFFE) + s);
            current_node->data = (uint8_t*)(current_node->data) - s;
            current_node->next = new_node;

            current_node->data = (uintptr_t)current_node->data & 1;

            break;
        }

        if (found_suitable_node)
        {
            break;
        }
        else
        {
            current_node = current_node->next;
        }
    }

    return (uintptr_t)current_node->next->data & 0xFFFFFFFE;
}

void kfree(void* p)
{
    LinkedList1Node* current_node = kernel_heap.allocs.head;
    bool found_node = false;
    for (uint64_t i = 0; i < kernel_heap.allocs.node_storage.count - 1; i++)
    {
        if (p <= ((uintptr_t)current_node->next->data & 0xFFFFFFFE)) // this is the allocation that needs to be freed
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
