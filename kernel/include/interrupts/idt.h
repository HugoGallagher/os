#pragma once

#include <stdint.h>
#include <stdbool.h>

struct idt_header;
struct idt_descriptor;

struct cpu_state;
struct stack_state;

typedef struct idt_header idt_header;
typedef struct idt_descriptor idt_descriptor;

typedef struct cpu_state cpu_state;
typedef struct stack_state stack_state;

struct idt_header
{
    uint16_t size;
    idt_descriptor* descriptors; // 256 long
} __attribute__((packed));

struct idt_descriptor
{
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t metadata;
    uint16_t offset_high;
} __attribute__((packed));

extern void idt_load(idt_header* ih);

void idt_fill_256(idt_header* ih);

idt_descriptor* idt_create_descriptor(idt_header* ih, uint8_t interrupt_code, uint32_t offset, bool in_memory, uint8_t dpl, bool gate_size, uint16_t segment_selector);

struct cpu_state
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

struct stack_state
{
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed));
