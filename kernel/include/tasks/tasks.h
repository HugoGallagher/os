#pragma once

#include "tasks/tss.h"
#include "mem/paging.h"
#include "lib/bitmap.h"

#include "files/fat32.h"

struct Task;
struct TaskAllocater;
struct TaskManager;

struct Registers;

typedef struct Task Task;
typedef struct TaskAllocater TaskAllocater;
typedef struct TaskManager TaskManager;

typedef struct Registers Registers;

struct Task
{
    // structure for page_dir:
    // 0 upwards: code
    // 767 downwards: stack (last pte is for kernel stack)
    // 768 upwards: kernel mapping

    PageDirectory* page_dir;
    uint32_t u_sp;
    uint32_t k_sp;
};
struct TaskAllocater
{
    uint32_t* pd_b;
    uint32_t pd_bc;
    PageDirectory* pds;

    Bitmap1* b2;
    uint32_t a;
    uint8_t bc;

    PageTable* pts;
};
struct TaskManager
{
    TSS* tss;

    Task* tasks;
    uint16_t count;

    TaskAllocater task_allocs;

    PageDirectory* k_page_dir;
};

void tm_init(TaskManager* tm, uint16_t c);

void tm_update_kpd(TaskManager* tm);

void tsk_init(Task* t, TaskManager* tm, uint32_t s, FAT32FS* fs, char* path, uint32_t path_size);

void ta_init(TaskAllocater* ta, uint32_t c);
PageTable* ta_alloc(TaskAllocater* ta);
void ta_free(TaskAllocater* ta, uint32_t index);
PageDirectory* ta_alloc_pd(TaskAllocater* ta);
void ta_free_pd(TaskAllocater* ta, uint32_t index);
