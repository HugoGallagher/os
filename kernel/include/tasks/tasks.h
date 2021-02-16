#pragma once

#include <stdbool.h>

#include "tasks/tss.h"
#include "mem/paging.h"
#include "lib/bitmap.h"

#include "files/fat32.h"

struct Registers;

struct Task;
struct TaskAllocater;
struct TaskManager;

typedef struct Registers Registers;

typedef struct Task Task;
typedef struct TaskAllocater TaskAllocater;
typedef struct TaskManager TaskManager;

struct Registers
{
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t esp_temp;
    uint32_t esi;
    uint32_t edi;

    uint32_t flags;

    uint32_t esp;
    uint32_t eip;
};

struct Task
{
    // structure for page_dir:
    // 0 upwards: code
    // 767 downwards: stack (currently the stack allocation is static), last pte is for message bus
    // 768 upwards: kernel

    uint16_t pcid;

    PageDirectory* page_dir;

    Registers registers;

    // if the task was preempted while in pl0
    bool in_pl0;
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
    uint32_t task_bitmaps[32];

    TSS* tss;

    Task* tasks;
    uint16_t count;

    TaskAllocater task_allocs;

    PageDirectory* k_page_dir;
};

static TaskManager task_manager;

void tm_init(uint16_t c);

void tm_switch_task(FAT32FS* fs, uint16_t id);
uint16_t tm_create_task(FAT32FS* fs, char* path, uint32_t path_size);

void tm_syscall(uint32_t index);

void tm_update_kpd();

TSS* tm_get_tss();

void tsk_init(Task* t, uint16_t id, FAT32FS* fs, char* path, uint32_t path_size);

void ta_init(uint32_t c);
PageTable* ta_alloc();
void ta_free(uint32_t index);
PageDirectory* ta_alloc_pd(ta);
void ta_free_pd(uint32_t index);

void enter_usr(Registers r);
void set_usr_srs();
void exit_usr(Registers rs);
uint32_t get_flags();
uint32_t get_esp();
