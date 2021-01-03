#pragma once

#include <stdint.h>

struct gdt_header;
struct gdt_descriptor;

typedef struct gdt_header gdt_header;
typedef struct gdt_descriptor gdt_descriptor;

struct gdt_header
{
    uint8_t* addr;
    uint16_t size;
} __attribute__((packed));

struct gdt_descriptor
{
    uint32_t base;
    uint32_t limit;
    uint32_t type;
};

extern void gdt_load(gdt_header gh);
extern void gdt_enable_protected_mode();

void gdt_init(gdt_header* gh, uint8_t* a);
void gdt_add_entry(gdt_header* gh, gdt_descriptor d);

extern void halt();
