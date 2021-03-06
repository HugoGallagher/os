#include <stddef.h>
#include <stdint.h>

#include "lib/linkedlist.h"

#include "interface/terminal.h"
#include "lib/core.h"
#include "lib/mem.h"

void ll1_init(LinkedList1* l, uint32_t size)
{
    size--;
    size += 8 - (size % 8);

    l->head = 0;
    l->tail = 0;
    uint8_t* p_nodes = kmalloc(size * sizeof(LinkedList1Node));
    uint8_t* p_nodes_allocs = kmalloc(size * 4);

    bzero(p_nodes, size * sizeof(LinkedList1Node));
    bzero(p_nodes_allocs, size * 4);

    l->node_storage.parent = l;
    l->node_storage.max_count = size;
    l->node_storage.count = 0;
    l->node_storage.nodes = p_nodes;
    l->node_storage.node_allocs = p_nodes_allocs;
}
void ll1_init_to_addr(LinkedList1* l, uint8_t* addr, uint32_t size)
{
    size--;
    size += 8 - (size % 8);

    l->head = 0;
    l->tail = 0;
    bzero(addr, size * sizeof(LinkedList1Node) + size * 4);

    uint8_t* p_allocs = addr + sizeof(LinkedList1Node) * size;

    l->node_storage.parent = l;
    l->node_storage.max_count = size;
    l->node_storage.count = 0;
    l->node_storage.nodes = addr;
    l->node_storage.node_allocs = p_allocs;
}

LinkedList1Node* ll1_push_front(LinkedList1* l, void* n)
{
    LinkedList1Node* node = ll1_ns_add(&(l->node_storage), n);
    //LinkedList1Node* node;

    node->next = l->head;
    l->head = node;

    return node;
}
LinkedList1Node* ll1_push_back(LinkedList1* l, void* n)
{
    if (l->node_storage.count == 0)
        return ll1_push_front(l, n);

    LinkedList1Node* node = ll1_ns_add(&(l->node_storage), n);

    LinkedList1Node* cn = l->head;
    for (int i = 0; i <= l->node_storage.count; i++)
    {
        if (cn->next == l->tail)
        {
            break;
        }

        cn = cn->next;
    }

    cn->next = node;
    node->next = l->tail;

    return node;
}

uint32_t ll1_push_front_return_index(LinkedList1* l, void* n)
{
    uint32_t index = ll1_ns_add_return_index(&(l->node_storage), n);
    LinkedList1Node* node = &(l->node_storage.nodes[index]);

    node->next = l->head;
    l->head = node;

    return index;
}
uint32_t ll1_push_back_return_index(LinkedList1* l, void* n)
{
    if (l->node_storage.count == 0)
        return ll1_push_front_return_index(l, n);

    uint32_t index = ll1_ns_add_return_index(&(l->node_storage), n);
    LinkedList1Node* node = &(l->node_storage.nodes[index]);

    LinkedList1Node* cn = l->head;
    for (int i = 0; i <= l->node_storage.count; i++)
    {
        if (cn->next == l->tail)
        {
            break;
        }

        cn = cn->next;
    }

    cn->next = node;
    node->next = l->tail;

    return index;
}

LinkedList1Node* ll1_insert(LinkedList1* l, LinkedList1Node* node, void* n)
{
    LinkedList1Node* n_node = ll1_ns_add(&(l->node_storage), n);
    n_node->next = node->next;
    node->next = n_node;

    return n_node;
}
uint32_t ll1_insert_return_index(LinkedList1* l, LinkedList1Node* node, void* n)
{
    uint32_t index = ll1_ns_add_return_index(&(l->node_storage), n);
    LinkedList1Node* n_node = &(l->node_storage.nodes[index]);

    n_node->next = node->next;
    node->next = n_node;

    return index;
}
void* ll1_remove_front(LinkedList1* l)
{
    if (l->head == 0)
    {
        return;
    }

    void* d = l->head->data;
    LinkedList1Node* nn = l->head->next;
    ll1_ns_remove(&(l->node_storage), l->head);
    l->head = nn;

    return d;
}
void* ll1_remove_back(LinkedList1* l)
{
    LinkedList1Node* cn = l->head;
    for (int i = 0; i < l->node_storage.count; i++)
    {
        cn = cn->next;

        if (cn->next->next = 0) // get penultimate node
            break;
    }

    void* d = cn->next->data;
    ll1_ns_remove(&(l->node_storage), cn->next);
    cn->next = 0;

    return d;
}

uint32_t ll1_remove_front_return_index(LinkedList1* l)
{
    if (l->head == 0)
    {
        return;
    }

    void* d = l->head->data;
    LinkedList1Node* nn = l->head->next;
    uint32_t index = ll1_ns_remove_return_index(&(l->node_storage), l->head);
    l->head = nn;

    return index;
}
uint32_t ll1_remove_back_return_index(LinkedList1* l)
{
    LinkedList1Node* cn = l->head;
    for (int i = 0; i < l->node_storage.count; i++)
    {
        cn = cn->next;

        if (cn->next->next = 0) // get penultimate node
            break;
    }

    void* d = cn->next->data;
    uint32_t index = ll1_ns_remove_return_index(&(l->node_storage), cn->next);
    cn->next = 0;

    return index;
}

void ll1_remove(LinkedList1* l, LinkedList1Node* n)
{
    if (l->head == 0)
    {
        return;
    }

    LinkedList1Node* cn = l->head;
    for (int i = 0; i < l->node_storage.count; i++)
    {
        cn = cn->next;

        if (cn->next = n)
            break;
    }

    LinkedList1Node* cn_next = cn->next->next;
    ll1_ns_remove(&(l->node_storage), cn->next);
    cn->next = cn_next;
}
uint32_t ll1_remove_return_index(LinkedList1* l, LinkedList1Node* n)
{
    if (l->head == 0)
    {
        return;
    }

    LinkedList1Node* cn = l->head;
    for (int i = 0; i < l->node_storage.count; i++)
    {
        cn = cn->next;

        if (cn->next = n)
            break;
    }

    LinkedList1Node* cn_next = cn->next->next;
    uint32_t index = ll1_ns_remove_return_index(&(l->node_storage), cn->next);

    cn->next = cn_next;
    return index;
}
LinkedList1Node* ll1_ns_add(LinkedList1NodeStorage* ns, void* n)
{
    for (uint32_t i = 0; i < ns->max_count; i++)
    {
        if (ns->node_allocs[i] == 0x7FFFFFFF) // this case seems to be required
        {
            LinkedList1Node* node = &(ns->nodes[(i*32)+31]);
            node->data = n;
            ns->count++;
            ns->node_allocs[i] = 0xFFFFFFFF;
            ns->nodes[(i*32)+31] = *node;
            return &(ns->nodes[(i*32)+31]);
        }
        else if (ns->node_allocs[i] != 0xFFFFFFFF)
        {
            for (uint32_t j = 0; j < 32; j++)
            {
                if (!(ns->node_allocs[i] & 1 << j))
                {
                    LinkedList1Node* node = &(ns->nodes[(i*32)+j]);
                    node->data = n;
                    ns->count++;
                    ns->node_allocs[i] |= 1 << j;
                    ns->nodes[(i*32)+j] = *node;
                    return &(ns->nodes[(i*32)+j]);
                }
            }
        }
    }

    return 0;
}
void ll1_ns_remove(LinkedList1NodeStorage* ns, LinkedList1Node* n)
{
    uint32_t index = n - ns->nodes;

    uint32_t offset = index % 64;
    index = (index - offset) / 64;

    ns->count--;

    ns->node_allocs[index] |= 1 << offset;
}

uint32_t ll1_ns_add_return_index(LinkedList1NodeStorage* ns, void* n)
{
    for (uint32_t i = 0; i < ns->max_count; i++)
    {
        if (ns->node_allocs[i] == 0x7FFFFFFF) // this case seems to be required
        {
            LinkedList1Node* node = &(ns->nodes[(i*32)+31]);
            node->data = n;
            ns->count++;
            ns->node_allocs[i] = 0xFFFFFFFF;
            ns->nodes[(i*32)+31] = *node;
            return i * 32 + 31;
        }
        else if (ns->node_allocs[i] != 0xFFFFFFFF)
        {
            for (uint32_t j = 0; j < 32; j++)
            {
                if (!(ns->node_allocs[i] & 1 << j))
                {
                    LinkedList1Node* node = &(ns->nodes[(i*32)+j]);
                    node->data = n;

                    ns->count++;
                    ns->node_allocs[i] |= 1 << j;
                    ns->nodes[(i*32)+j] = *node;
                    return i * 32 + j;
                }
            }
        }
    }

    return 0;
}
uint32_t ll1_ns_remove_return_index(LinkedList1NodeStorage* ns, LinkedList1Node* n)
{
    uint32_t index = n - ns->nodes;

    uint32_t offset = index % 64;
    index = (index - offset) / 64;

    ns->count--;

    ns->node_allocs[index] |= 1 << offset;

    return index;
}

void ll2_init(LinkedList2* l, uint32_t size)
{
    size += 8 - (size % 8);

    l->head = 0;
    l->tail = 0;

    uint8_t* addr = kmalloc(size * sizeof(LinkedList2Node) + size / 8);

    bzero(addr, size * sizeof(LinkedList2Node) + size / 8);

    uint8_t* p_allocs = addr + sizeof(LinkedList2Node) * size;

    l->node_storage.parent = l;
    l->node_storage.count = 0;
    l->node_storage.max_count = size;
    l->node_storage.count = 0;
    l->node_storage.nodes = addr;
    l->node_storage.node_allocs = p_allocs;
}
void ll2_init_to_addr(LinkedList2* l, uint8_t* addr, uint32_t size)
{
    size += 8 - (size % 8);

    l->head = 0;
    l->tail = 0;

    bzero(addr, size * sizeof(LinkedList2Node) + size / 8);

    uint8_t* p_allocs = addr + sizeof(LinkedList2Node) * size;

    l->node_storage.parent = l;
    l->node_storage.count = 0;
    l->node_storage.max_count = size;
    l->node_storage.count = 0;
    l->node_storage.nodes = addr;
    l->node_storage.node_allocs = p_allocs;
}

LinkedList2Node* ll2_push_front(LinkedList2* l, void* n)
{
    LinkedList2Node* node = ll2_ns_add(&(l->node_storage), n);

    if (l->head == 0)
    {
        l->tail = node;
    }

    node->next = l->head;
    node->prev = NULL;
    l->head = node;

    return node;
}
LinkedList2Node* ll2_push_back(LinkedList2* l, void* n)
{
    LinkedList2Node* node = ll2_ns_add(&(l->node_storage), n);

    if (l->head == 0)
    {
        l->head = node;
    }

    node->prev = l->tail;
    node->next = NULL;
    l->tail = node;

    return node;
}
LinkedList2Node* ll2_insert(LinkedList2* l, LinkedList2Node* node, void* n)
{
    LinkedList2Node* n_node = ll2_ns_add(&(l->node_storage), n);
    n_node->next = node->next;
    n_node->prev = node;
    node->next = n_node;

    return n_node;
}
void* ll2_remove_front(LinkedList2* l)
{
    if (l->head == 0)
    {
        return;
    }
    if (l->head == l->tail)
    {
        LinkedList2Node* n = l->head;

        l->head = NULL;
        l->tail = NULL;

        return n;
    }

    LinkedList2Node* n = l->head;

    l->head = n->next;

    ll2_ns_remove(&(l->node_storage), n);

    return n->data; // this can be safely returned since node data isn't overwritten in ll2_ns_remove
}
void* ll2_remove_back(LinkedList2* l)
{
    if (l->head == 0)
    {
        return;
    }
    if (l->head == l->tail)
    {
        LinkedList2Node* n = l->tail;

        l->head = NULL;
        l->tail = NULL;

        return n;
    }

    LinkedList2Node* n = l->tail;

    l->tail = n->prev;

    ll2_ns_remove(&(l->node_storage), n);

    return n->data;
}
void* ll2_remove(LinkedList2* l, LinkedList2Node* n)
{
    if (l->head == 0)
    {
        return;
    }
    if (l->head == l->tail)
    {
        l->head = NULL;
        l->tail = NULL;

        return;
    }
    if (l->head->next == l->tail)
    {
        if (l->head = n)
        {
            l->head = n->next;
        }
        else
        {
            l->tail = n->prev;
        }

        ll2_ns_remove(&(l->node_storage), n);

        return;
    }

    (n->prev)->next = n->next;
    (n->next)->prev = n->prev;

    n->next = NULL;
    n->prev = NULL;

    ll2_ns_remove(&(l->node_storage), n);

    return n->data;
}
LinkedList2Node* ll2_ns_add(LinkedList2NodeStorage* ns, void* n)
{
    for (uint32_t i = 0; i < ns->max_count; i++)
    {
        if (ns->node_allocs[i] != 0xFFFFFFFF)
        {
            for (uint32_t j = 0; j < 32; j++)
            {
                if (!(ns->node_allocs[i] & 1 << j))
                {
                    LinkedList2Node* node = &(ns->nodes[(i*32)+j]);
                    node->data = n;

                    ns->count++;
                    ns->node_allocs[i] |= 1 << j;
                    ns->nodes[(i*32)+j] = *node;

                    return &(ns->nodes[(i*32)+j]);
                }
            }
        }
    }

    return 0;
}
void ll2_ns_remove(LinkedList2NodeStorage* ns, LinkedList2Node* n)
{
    LinkedList2Node* identity_pointer = n - ns->nodes;
    uint32_t index = *(uint32_t*)&identity_pointer / sizeof(LinkedList2Node);

    uint32_t offset = index % 64;
    index -= index & 64;

    ns->node_allocs[index] |= 1 << offset;
}
