#include "interface/cli.h"

#include "interface/terminal.h"

char sc_to_ascii_table[] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'
};

void cli_send_char(char c)
{
    terminal_putchar(sc_to_ascii(c));
}

char sc_to_ascii(char c)
{
    return sc_to_ascii_table[c];
}
