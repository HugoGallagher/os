#pragma once

uint8_t inb(uint16_t p);
void outb(uint16_t p, uint8_t d);

uint16_t inw(uint16_t p);
void outw(uint16_t p, uint16_t d);

uint32_t inl(uint32_t p);
void outl(uint16_t p, uint32_t d);
