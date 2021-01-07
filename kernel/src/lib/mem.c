#include <stddef.h>
#include <stdint.h>

#include "lib/mem.h"

void heap_init(Heap* heap, uint64_t size, void* start)
{
    if ((size % 16) != 0) { return; }

    heap->data = start;
    heap->data_size = size;

    uint64_t alloc_count = size / 16;

    heap->nodes = size + (uint64_t)start;
    heap->max_allocations = alloc_count / 16;
    // ^ this saves memory by assuming that the heap will never reach a certain point of fragmentation
    // very bad idea, should later be changed to save memory via a case-optimised linked list

    bzero(heap->data, heap->data_size);
    bzero(heap->nodes, heap->max_allocations * sizeof(LinkedList1Node));

    heap->nodes->data = (uint64_t)HEAP_SIZE;
    ll1_push_front(&(heap->spaces), heap->nodes);

    //ll_push_front(heap->allocations, *(heap->nodes));
}

// uses kernel_heap
// finds a 16-byte aligned space in heap memory large enough to store the variable
// if there isnt a space large enough it will just return 0
// heap allocations are stored in a linked list, which tracks gaps in the heap
void* kmalloc(uint64_t s)
{
    s += 16 - (s % 16); // align to 16 bytes
    uint64_t segments_needed = s / 16;

    LinkedList1Node* current_node = kernel_heap.nodes;
    bool found_suitable_node = false;
    for (uint64_t i = 0; i < kernel_heap.max_allocations; i++)
    {
        if ((uint64_t)(current_node->data) >= s)
        {
            found_suitable_node = true;

            current_node->data = (uint64_t)(current_node->data) - s;
            if ((uint64_t)(current_node->data) == 0)
            {
                ll1_remove(&(kernel_heap.spaces), current_node);
            }

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
}

void kfree(void* p)
{

}
