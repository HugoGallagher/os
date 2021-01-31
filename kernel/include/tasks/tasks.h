#pragma once

#include "tasks/tss.h"
#include "mem/paging.h"

struct Task;
struct TaskManager;

struct Registers;

typedef struct Task Task;
typedef struct TaskManager TaskManager;

typedef struct Registers Registers;

struct Task
{
    PageDirectory page_dir;
    uint32_t u_sp;
    uint32_t k_sp;
};
struct TaskManager
{
    TSS* tss;

    Task* tasks;
    uint16_t count;

    PageDirectory* k_page_dir;
};

struct Registers
{

};

void tm_init(TaskManager* tm, uint16_t c);
void tsk_init(Task* t, TaskManager* tm);
