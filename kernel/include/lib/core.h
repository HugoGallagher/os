#pragma once

#include <stddef.h>
#include <stdint.h>

extern uint8_t _kernel_end;
static void* kernel_end;

void bzero(void *s, size_t n);
void* memcpy(void* restrict to, const void* restrict from, size_t size);
size_t strlen(const char* str);
