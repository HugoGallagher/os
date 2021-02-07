#include "lib/core.h"

#include "interface/terminal.h"

void bzero(void *s, size_t n)
{
    uint8_t* s_as_array = s;

    for (size_t i = 0; i <= (n - 1); i++)
    {
        //terminal_writehex(s + i);
        s_as_array[i] = 0;
    }
}

void* memcpy(void* restrict to, const void* restrict from, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        *(((uint8_t*)to) + i) = *(((const uint8_t*)from) + i);
    }
}

int memcmp(const char* str1, const char* str2, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (str1[i] != str2[i])
            return 0;
    }
    return 1;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
