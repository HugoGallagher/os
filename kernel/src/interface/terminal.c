#include "interface/terminal.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xC03FF000;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
		for (size_t x = 0; x < VGA_WIDTH; x++)
        {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH)
    {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
    {
        if (data[i] == '\n')
        {
            terminal_row++;
            terminal_column = 0;
        }
        else
        {
            terminal_putchar(data[i]);
        }
    }

    terminal_check();
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_writehex(void* data)
{
    static char hex_values[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    for (int i = 7; i >= 0; i--)
    {
        terminal_putchar(hex_values[((uint32_t)data >> (i * 4)) & 0b1111]);
    }

    terminal_row++;
    terminal_column = 0;

    terminal_check();
}

void terminal_check()
{
    if (terminal_row > VGA_HEIGHT)
	//if (terminal_row > 0)
    {
//		for (int i = 0; i < VGA_HEIGHT; i++)
//		{
//			for (int j = 0; j < VGA_WIDTH; j++)
//			{
//				terminal_putentryat(0, terminal_color, j, i);
//			}
//		}

        terminal_row = 0;
		terminal_column = 0;
    }
}
