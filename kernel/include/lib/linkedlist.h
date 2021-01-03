#pragma once

#include <stddef.h>
#include <stdint.h>

struct LLNode;
struct LinkedList;

typedef struct LLNode LLNode;
typedef struct LinkedList LinkedList;

struct LLNode
{
    void* data;
    LLNode* next;
    LLNode* prev;
};

struct LinkedList // nodes still need to be stored externally
{
    LLNode* head; // at "front" of list
    LLNode* tail;
};

void ll_push_front(LinkedList* l, LLNode* n);
void ll_push_back(LinkedList* l, LLNode* n);
void* ll_remove_front(LinkedList* l);
void* ll_remove_back(LinkedList* l);
void* ll_remove(LinkedList* l, LLNode* n);
