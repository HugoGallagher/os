#include <stdint.h>

#include "asm_io.h"

uint8_t inb(uint16_t p)
{
    uint8_t r;
    asm volatile("inb %1, %0" : "=a"(r) : "Nd"(p));

    return r;
}
void outb(uint16_t p, uint8_t d)
{
    asm volatile("outb %0, %1" : : "a"(d), "Nd"(p));
}

uint16_t inw(uint16_t p)
{
    uint16_t r;
    asm volatile("inw %w1, %0" : "=a"(r) : "Nd"(p));

    return r;
}
void outw(uint16_t p, uint16_t d)
{
    asm volatile("outw %0, %w1" : : "a"(d), "Nd"(p));
}

uint32_t inl(uint32_t p)
{
    uint32_t r;
    asm volatile("inl %d1, %0" : "=a"(r) : "Nd"(p));

    return r;
}
void outl(uint16_t p, uint32_t d)
{
    asm volatile("outl %0, %d1" : : "a"(d), "Nd"(p));
}
