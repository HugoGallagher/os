#include <stddef.h>
#include <stdint.h>

#include "lib/linkedlist.h"

void ll_push_front(LinkedList* l, LLNode* n)
{
    if (l->head == 0)
    {
        l->tail = n;
    }

    n->next = l->head;
    n->prev = NULL;
    l->head = n;
}
void ll_push_back(LinkedList* l, LLNode* n)
{
    if (l->head == 0)
    {
        l->head = n;
    }

    n->prev = l->tail;
    n->next = NULL;
    l->tail = n;
}
void* ll_remove_front(LinkedList* l)
{
    if (l->head == 0)
    {
        return;
    }
    if (l->head == l->tail)
    {
        LLNode* n = l->head;

        n->next = NULL;
        n->prev = NULL;
        l->head = NULL;
        l->tail = NULL;

        return n;
    }

    LLNode* n = l->head;

    l->head = n->next;
    n->next = NULL;
    n->prev = NULL;

    return n;
}
void* ll_remove_back(LinkedList* l)
{
    if (l->head == 0)
    {
        return;
    }
    if (l->head == l->tail)
    {
        LLNode* n = l->tail;

        n->next = NULL;
        n->prev = NULL;
        l->head = NULL;
        l->tail = NULL;

        return n;
    }

    LLNode* n = l->tail;

    l->tail = n->prev;
    n->next = NULL;
    n->prev = NULL;

    return n;
}
void* ll_remove(LinkedList* l, LLNode* n)
{
    if (l->head == 0)
    {
        n->next = NULL;
        n->prev = NULL;

        return;
    }
    if (l->head == l->tail)
    {
        n->next = NULL;
        n->prev = NULL;
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

        n->next = NULL;
        n->prev = NULL;

        return;
    }

    (n->prev)->next = n->next;
    (n->next)->prev = n->prev;

    n->next = NULL;
    n->prev = NULL;

    return n;
}
