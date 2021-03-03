#pragma once

#include <stdbool.h>

#include "tasks/tss.h"
#include "interrupts/idt.h"
#include "mem/paging.h"
#include "lib/bitmap.h"

#include "files/fat32.h"

// soft limit of 10 servers
#define SERVER_COUNT 3
enum ServerType
{
    SV_DISPLAY,
    SV_INPUT,
    SV_FILESYSTEM,
};

typedef enum ServerType ServerType;

struct MessageBus;
struct Registers;

struct Task;
struct TaskAllocater;
struct TaskManager;

typedef struct MessageBus MessageBus;
typedef struct Registers Registers;

typedef struct Task Task;
typedef struct TaskAllocater TaskAllocater;
typedef struct TaskManager TaskManager;

// messages are stored in a stack-like structure
#define MAX_MSG_SIZE 64
#define MAX_MSG_DATA 3968
struct MessageBus
{
    uint16_t lengths[MAX_MSG_SIZE];
    uint8_t data[MAX_MSG_DATA];
} __attribute__((packed));

struct Registers
{
    uint32_t edi;
    uint32_t esi;
    uint32_t esp_temp;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

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

    uint32_t size;
    PageTable* pt_stack;

    FAT32FS* fs;

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
    uint16_t servers[SERVER_COUNT];
    uint16_t count;
    uint16_t current_task;

    FAT32FS* fs_primary;

    TaskAllocater task_allocs;

    PageDirectory* k_page_dir;

    bool multitasking;
};

static TaskManager task_manager;

void tm_init(uint16_t c);

void tm_init_servers(FAT32FS* fs, char* path, uint32_t path_size);
void tm_init_startup(FAT32FS* fs, char* path, uint32_t path_size);

void tm_enter_next_task();
void tm_enter_task(uint16_t id);

uint16_t tm_create_task(bool is_server, char* path, uint32_t path_size);
uint16_t tm_create_task_from_fs(FAT32FS* fs, bool is_server, char* path, uint32_t path_size);
void tm_delete_active_task();

void tm_msg_transmit(uint32_t dst, uint8_t* data, uint32_t len);
void tm_msg_get(uint8_t** p_data, uint32_t* len);
void tm_msg_ack();

void tm_save_registers(GeneralRegisters r, uint32_t eip, uint32_t esp);

void tm_preempt_pl0(GeneralRegisters r, uint32_t eip, uint16_t cs, uint32_t f);
void tm_preempt_pl3(GeneralRegisters r, uint32_t eip, uint16_t cs, uint32_t f, uint32_t esp, uint16_t ss);

TSS* tm_get_tss();
bool tm_is_multitasking();
uint32_t tm_get_task_stack_base();
uint16_t tm_get_current_task();
uint16_t tm_get_server_id(ServerType type);
void tm_set_fs(FAT32FS* fs);

void tsk_init(Task* t, uint16_t id, FAT32FS* fs, bool is_server, char* path, uint32_t path_size);

void ta_init(uint32_t c);
PageTable* ta_alloc();
void ta_free(uint32_t index);
PageDirectory* ta_alloc_pd(ta);
void ta_free_pd(uint32_t index);

void enter_usr(Registers r);
void set_usr_srs();
void exit_usr(Registers rs);

uint32_t get_eip();
uint32_t get_flags();
uint32_t get_esp();
uint32_t get_cr2();
