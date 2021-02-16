#include "mem/paging.h"

// currently this will cause a page fault if an invalid v_addr is passed in
void* pg_get_phys_addr(uint32_t v_addr)
{
    uint32_t i_pd = v_addr >> 22;
    uint32_t i_pt = (v_addr >> 12) & 0x3FF;
    uint32_t* pt = 0xFFC00000 + i_pd * 0x1000;

    return (pt[i_pt] & 0xFFFFF000) + (v_addr & 0x00000FFF);

}
PageDirectoryEntry* pg_get_pde(uint32_t pdi)
{
    return 0xFFFFF000 + pdi * 4;
}
PageTableEntry* pg_get_pte(uint32_t pti, uint32_t pdi)
{
    return 0xFFC00000 + pti * 0x1000 + pdi * 4;
}

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

void pg_load_pd(PageDirectory* pd)
{
    load_cr3(pg_get_phys_addr(pd));
}
