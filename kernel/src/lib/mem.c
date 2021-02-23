#include <stddef.h>
#include <stdint.h>

#include "lib/mem.h"
#include "lib/core.h"

#include "interface/terminal.h"

Heap kheap;

void kheap_init(void* start)
{
    start += 64;
    start = (uint32_t)start & ~(0b111111);

    kheap.start = start;

    uint32_t available_memory = pa_get_available_memory();

    // arbitrary, room for optimisation
    kheap.block_size = 4*1024*1024;
    kheap.block_node_count = (4*1024);
    kheap.block_count = 0;

    kheap_add_pt();
    ll1_init_to_addr(&(kheap.allocs), kheap.start, kheap.block_node_count);

    kheap.alloc_data = start + kheap.block_node_count * (sizeof(LinkedList1Node) + 4);

    // since kmalloc() depends on a few pre-existing allocations, these need to be set manually

    HeapAllocation* blank_alloc = kheap.alloc_data + 0; // reference point for start of memory block
    HeapAllocation* alloc_data_1 = kheap.alloc_data + 1; // the ll nodes alloc
    HeapAllocation* alloc_data_2 = kheap.alloc_data + 2; // the ll node_alloc alloc
    HeapAllocation* alloc_data_3 = kheap.alloc_data + 3; // the heap alloc_data alloc

    blank_alloc->data = start;
    alloc_data_1->data = kheap.allocs.node_storage.nodes;
    alloc_data_2->data = kheap.allocs.node_storage.node_allocs;
    alloc_data_3->data = alloc_data_2->data + kheap.block_node_count * 4;

    blank_alloc->size = 0;
    alloc_data_1->size = kheap.block_node_count * sizeof(LinkedList1Node);
    alloc_data_2->size = kheap.block_node_count * 4;
    alloc_data_3->size = kheap.block_node_count * sizeof(HeapAllocation);

    ll1_push_front(&(kheap.allocs), alloc_data_3);
    ll1_push_front(&(kheap.allocs), alloc_data_2);
    ll1_push_front(&(kheap.allocs), alloc_data_1);
    ll1_push_front(&(kheap.allocs), blank_alloc);
}

void kheap_push_block()
{
    // preserve old data to copy and then free
    void* old_nodes = kheap.allocs.node_storage.nodes;
    void* old_node_allocs = kheap.allocs.node_storage.node_allocs;

    ll1_init(&(kheap.allocs), kheap.block_count * kheap.block_node_count);
    memcpy(kheap.allocs.node_storage.nodes, old_nodes, (kheap.block_count - 1) * kheap.block_node_count * sizeof(LinkedList1Node));
    memcpy(kheap.allocs.node_storage.node_allocs, old_node_allocs, (kheap.block_count - 1) * kheap.block_node_count * sizeof(uint32_t));

    kfree(old_nodes);
    kfree(old_node_allocs);

    void* old_alloc_data = kheap.alloc_data;

    kheap.alloc_data = kmalloc(kheap.block_count * kheap.block_node_count);
    memcpy(kheap.allocs.node_storage.nodes, old_alloc_data, (kheap.block_count - 1) * kheap.block_node_count * 4);

    kfree(old_alloc_data);
}

void kheap_add_pt()
{
    uint16_t flags = (1 << 0) | (1 << 1);
    kheap.block_count++;

    uint32_t t_addr = pa_alloc();
    PageDirectoryEntry* pde = pg_get_pde(767 + KERNEL_PAGE_TABLES + kheap.block_count);
    pde->data = t_addr | flags;

    for (uint32_t i = 0; i < 1024; i++)
    {
        uint32_t te_addr = pa_alloc();
        PageTableEntry* pte = pg_get_pte(767 + KERNEL_PAGE_TABLES + kheap.block_count, i);
        pte->data = te_addr | flags;
    }
}

void* kmalloc(uint32_t s)
{
    if (kheap.allocs.node_storage.count == kheap.allocs.node_storage.max_count)
        kheap_push_block();

    s += 63;
    s &= ~(0b111111);

    uint32_t size = s;
    //terminal_writehex(size);

    LinkedList1Node* current_node = kheap.allocs.head;
    LinkedList1Node* next_node = current_node->next;
    HeapAllocation* current_alloc = current_node->data;
    HeapAllocation* next_alloc = next_node->data;
    HeapAllocation* new_alloc;
    bool found_node = false;
    while (current_node->next != kheap.allocs.tail)
    {
        if (s <= (next_alloc->data - (current_alloc->data + current_alloc->size)))
        {
            // there is space after all allocations
            uint32_t index = ll1_insert_return_index(&(kheap.allocs), current_node, 0);
            new_alloc = kheap.alloc_data + index;
            kheap.allocs.node_storage.nodes[index].data = current_alloc;

            new_alloc->size = s;
            new_alloc->data = next_alloc->data + next_alloc->size;

            return new_alloc->data;
        }

        current_node = current_node->next;
        next_node = current_node->next;
        current_alloc = current_node->data;
        next_alloc = next_node->data;
    }

    if (kheap.block_count * kheap.block_size > current_alloc->data + current_alloc->size + s - 0xC0000000)
    {
        uint32_t index = ll1_insert_return_index(&(kheap.allocs), current_node, 0);
        new_alloc = kheap.alloc_data + index;
        kheap.allocs.node_storage.nodes[index].data = new_alloc;

        next_node = current_node->next;
        new_alloc = next_node->data;

        new_alloc->size = s;
        new_alloc->data = current_alloc->data + current_alloc->size;

        return new_alloc->data;
    }
    else
    {
        // if there wasn't space for the allocation add the necessary page tables

        // get the number of additional page tables needed
        uint32_t page_tables_needed = s + kheap.block_size - 1;
        page_tables_needed &= ~(0b1111111111111111111111);
        page_tables_needed /= kheap.block_size;

        for (uint32_t i = 0; i < page_tables_needed; i++)
        {
            kheap_add_pt();
        }

        uint32_t index = ll1_insert_return_index(&(kheap.allocs), current_node, 0);
        new_alloc = kheap.alloc_data + index;
        kheap.allocs.node_storage.nodes[index].data = new_alloc;

        next_node = current_node->next;
        new_alloc = next_node->data;

        new_alloc->size = s;
        new_alloc->data = current_alloc->data + current_alloc->size;

        return new_alloc->data;
    }
}

void kfree(void* p)
{
    LinkedList1Node* current_node = kheap.allocs.head;
    HeapAllocation* current_alloc;
    while (current_node != kheap.allocs.tail)
    {
        current_alloc = current_node->data;
        if (current_alloc->data == p)
        {
//            uint32_t index = ll1_remove_return_index(&(kheap.allocs), current_node);
//            current_alloc = kheap.alloc_data + index;
//            current_alloc->data = 0;
//            current_alloc->size = 0;

            return;
        }

        current_node = current_node->next;
    }
}
