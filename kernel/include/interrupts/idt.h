#pragma once

#include <stdint.h>
#include <stdbool.h>

struct IDTHeader;
struct IDTDescriptor;

struct GeneralRegisters;

typedef struct IDTHeader IDTHeader;
typedef struct IDTDescriptor IDTDescriptor;

typedef struct GeneralRegisters GeneralRegisters;

struct IDTHeader
{
    uint16_t size;
    IDTDescriptor* descriptors; // 256 long
} __attribute__((packed));

struct IDTDescriptor
{
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t metadata;
    uint16_t offset_high;
} __attribute__((packed));

extern void idt_load(IDTHeader* ih);

void idt_fill_256(IDTHeader* ih);

IDTDescriptor* idt_create_descriptor(IDTHeader* ih, uint8_t interrupt_code, uint32_t offset, bool in_memory, uint8_t dpl, bool gate_size, uint16_t segment_selector);

struct GeneralRegisters
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} __attribute__((packed));
