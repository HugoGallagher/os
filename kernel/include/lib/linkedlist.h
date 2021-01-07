#pragma once

#include <stddef.h>
#include <stdint.h>

struct LinkedList1Node;
struct LinkedList1;
struct LinkedList1NodeStorage;

struct LinkedList2Node;
struct LinkedList2;
struct LinkedList2NodeStorage;

typedef struct LinkedList1Node LinkedList1Node;
typedef struct LinkedList1 LinkedList1;
typedef struct LinkedList1NodeStorage LinkedList1NodeStorage;

typedef struct LinkedList2Node LinkedList2Node;
typedef struct LinkedList2 LinkedList2;
typedef struct LinkedList2NodeStorage LinkedList2NodeStorage;

struct LinkedList1Node
{
    void* data;
    LinkedList1Node* next;
};
struct LinkedList1NodeStorage // this idea sort of goes against the purpose of a linked list, but there isn't really a better way to handle this
{
    uint32_t max_count;
    uint32_t count;

    LinkedList1Node* nodes;
    uint64_t* node_allocs;
};
struct LinkedList1
{
    LinkedList1Node* head;
    void* tail;

    LinkedList1NodeStorage node_storage;
};

struct LinkedList2Node
{
    void* data;
    LinkedList2Node* next;
    LinkedList2Node* prev;
};
struct LinkedList2NodeStorage
{
    uint32_t max_count;
    uint32_t count;

    LinkedList2Node* nodes;
    uint64_t* node_allocs;
};
struct LinkedList2
{
    LinkedList2Node* head; // at "front" of list
    LinkedList2Node* tail;

    LinkedList2NodeStorage node_storage;
};

void ll1_init(LinkedList1* l, uint32_t size, uint8_t* addr);

void ll1_push_front(LinkedList1* l, LinkedList1Node* n);
void ll1_push_back(LinkedList1* l, LinkedList1Node* n);
void* ll1_remove_front(LinkedList1* l);
void* ll1_remove_back(LinkedList1* l);
void* ll1_remove(LinkedList1* l, LinkedList1Node* n);

LinkedList1Node* ll1_ns_add(LinkedList1NodeStorage* ns, LinkedList1Node* n);
void ll1_ns_remove(LinkedList1NodeStorage* ns, LinkedList1Node* n);

void ll2_init(LinkedList2* l, uint32_t size, uint8_t* addr);

void ll2_push_front(LinkedList2* l, LinkedList2Node* n);
void ll2_push_back(LinkedList2* l, LinkedList2Node* n);
void* ll2_remove_front(LinkedList2* l);
void* ll2_remove_back(LinkedList2* l);
void* ll2_remove(LinkedList2* l, LinkedList2Node* n);

LinkedList1Node* ll2_ns_add(LinkedList2NodeStorage* ns, LinkedList2Node* n);
void ll2_ns_remove(LinkedList2NodeStorage* ns, LinkedList2Node* n);
