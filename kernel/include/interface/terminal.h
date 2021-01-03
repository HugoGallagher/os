#pragma once

#include <stddef.h>
#include <stdint.h>

#include "graphics/vga.h"

void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_writehex(void* data);
void terminal_check();
