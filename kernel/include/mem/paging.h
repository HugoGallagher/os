#pragma once

#include <stdint.h>
#include <stdbool.h>

#define KERNEL_PAGE_TABLES 0x1

enum pde_flags
{
    pde_present = 0,
    pde_read,
    pde_user,
    pde_writethrough,
    pde_cachedisabled,
    pde_accessed,
    pde_size,
    pde_ignored,
    pde_reserved1,
    pde_reserved2,
};
enum pte_flags
{
    pte_present = 0,
    pte_read,
    pte_user,
    pte_writethrough,
    pte_cachedisabled,
    pte_accessed,
    pte_dirty,
    pte_global,
    pte_reserved1,
    pte_reserved2,
};

struct PageDirectoryEntry;
struct PageTableEntry;
struct PageDirectory;
struct PageTable;

typedef struct PageDirectoryEntry PageDirectoryEntry;
typedef struct PageTableEntry PageTableEntry;
typedef struct PageDirectory PageDirectory;
typedef struct PageTable PageTable;

struct PageDirectoryEntry
{
    uint32_t data;
};
struct PageTableEntry
{
    uint32_t data;
};
struct PageDirectory
{
    PageDirectoryEntry entries[1024];
};
struct PageTable
{
    PageTableEntry entries[1024];
};

extern void pg_enable(PageDirectoryEntry* pd);

void* pg_get_phys_addr(uint32_t v_addr);
PageDirectoryEntry* pg_get_pde(uint32_t pdi);
PageTableEntry* pg_get_pte(uint32_t pdi, uint32_t pti);

void pde_set_flag(PageDirectoryEntry* pde, enum pde_flags f, bool v);
bool pde_get_flag(PageDirectoryEntry* pde, enum pde_flags f);
void pde_set_addr(PageDirectoryEntry* pde, void* addr);
void* pde_get_addr(PageDirectoryEntry* pde);

void pte_set_flag(PageTableEntry* pte, enum pte_flags f, bool v);
bool pte_get_flag(PageTableEntry* pte, enum pte_flags f);
void pte_set_addr(PageTableEntry* pte, void* addr);
void* pte_get_addr(PageTableEntry* pte);

void pg_load_cr3(PageDirectory* pd);
