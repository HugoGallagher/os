#pragma once

#include <stdint.h>

struct GDTHeader;
struct GDTDescriptor;

typedef struct GDTHeader GDTHeader;
typedef struct GDTDescriptor GDTDescriptor;

struct GDTHeader
{
    uint8_t* addr;
    uint16_t size;
} __attribute__((packed));

struct GDTDescriptor
{
    uint32_t base;
    uint32_t limit;
    uint32_t type;
};

extern void gdt_load(GDTHeader gh);
extern void gdt_reload_cs();

void gdt_init(GDTHeader* gh, uint8_t* a);
void gdt_add_entry(GDTHeader* gh, GDTDescriptor d);

extern void halt();
