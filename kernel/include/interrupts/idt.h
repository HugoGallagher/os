#pragma once

#include <stdint.h>
#include <stdbool.h>

struct IDTHeader;
struct IDTDescriptor;

struct CPUState;
struct StackState;

typedef struct IDTHeader IDTHeader;
typedef struct IDTDescriptor IDTDescriptor;

typedef struct CPUState CPUState;
typedef struct StackState StackState;

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

struct CPUState
{
    // might also need segment registers
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
} __attribute__((packed));

struct StackState
{
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed));
