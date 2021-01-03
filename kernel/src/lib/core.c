#include "lib/core.h"

#include "interface/terminal.h"

void bzero(void *s, size_t n)
{
    for (size_t i = 0; i <= (n - 1); i++)
    {
        //terminal_writehex(s + i);
        *(uint8_t*)(s + i) = 0;
    }
}

void* memcpy(void* restrict to, const void* restrict from, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        *(((uint8_t*)to) + i) = *(((const uint8_t*)from) + i);
    }
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
