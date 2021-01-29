#pragma once

#include "tasks/tss.h"
#include "mem/paging.h"

struct Task;
struct TaskManager;

typedef struct Task Task;
typedef struct TaskManager TaskManager;

struct Task
{
    PageDirectory page_dir;
    uint32_t ss;
    uint32_t esp;
};
struct TaskManager
{
    TSS* tss;

    Task* tasks;
    uint16_t count;
};

void tm_init(TaskManager* tm, uint16_t c);
