#include "mem/paging.h"

void pde_set_flag(PageDirectoryEntry* pde, enum pde_flags f, bool v)
{
    pde->data = (pde->data & ~(1 << f)) | 1 << v;
}
bool pde_get_flag(PageDirectoryEntry* pde, enum pde_flags f)
{
    return pde->data & 1 << f;
}

void pde_set_addr(PageDirectoryEntry* pde, void* addr)
{
    pde->data |= (*(uint32_t*)&addr & 0xFFFFF000);
}
void* pde_get_addr(PageDirectoryEntry* pde)
{
    return pde->data & 0xFFFFF000;
}

void pte_set_flag(PageTableEntry* pte, enum pte_flags f, bool v)
{
    pte->data = (pte->data & ~(1 << f)) | 1 << v;
}
bool pte_get_flag(PageTableEntry* pte, enum pte_flags f)
{
    return pte->data & 1 << f;
}

void pte_set_addr(PageTableEntry* pte, void* addr)
{
    pte->data |= (*(uint32_t*)&addr & 0xFFFFF000);
}
void* pte_get_addr(PageTableEntry* pte)
{
    return pte->data & 0xFFFFF000;
}
