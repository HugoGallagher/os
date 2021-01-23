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
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t limit_high_flags;
    uint8_t base_high;
} __attribute__((packed));

extern void gdt_load(GDTHeader gh);
extern void gdt_reload_cs();

void gdt_init(GDTHeader* gh, uint8_t* a);
void gdt_create_descriptor(GDTDescriptor* gd, uint32_t base, uint32_t limit, uint8_t present, uint8_t privilege, uint8_t type, uint8_t executable, uint8_t dir_conf, uint8_t read_write);
void gdt_add_entry(GDTHeader* gh, GDTDescriptor d);

extern void halt();
