#pragma once

#include <stdint.h>
#include <stdbool.h>

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

struct PageDirEntry;
struct PageTableEntry;
struct PageTable;

typedef struct PageDirEntry PageDirEntry;
typedef struct PageTableEntry PageTableEntry;
typedef struct PageTable PageTable;

struct PageDirEntry
{
    uint32_t data;
};
struct PageTableEntry
{
    uint32_t data;
};
struct PageTable
{
    PageTableEntry entries[1024];
};

extern void pg_enable(PageDirEntry* pd);

void pde_set_flag(PageDirEntry* pde, enum pde_flags f, bool v);
bool pde_get_flag(PageDirEntry* pde, enum pde_flags f);
void pde_set_addr(PageDirEntry* pde, void* addr);
void* pde_get_addr(PageDirEntry* pde);

void pte_set_flag(PageTableEntry* pte, enum pte_flags f, bool v);
bool pte_get_flag(PageTableEntry* pte, enum pte_flags f);
void pte_set_addr(PageTableEntry* pte, void* addr);
void* pte_get_addr(PageTableEntry* pte);
