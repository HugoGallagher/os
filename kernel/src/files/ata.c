#include "files/ata.h"

#include "lib/core.h"
#include "lib/mem.h"

#include "asm_io.h"
#include "interface/terminal.h"

void* ata_read(uint32_t lba, uint8_t sc)
{
    uint16_t* data = kmalloc(sc * 512);

    lba = lba & 0x0FFFFFFF;

    uint8_t b_lba;

    outb(0x1F6, 0xE0 | ((lba >> 24) & 0xF));

    outb(0x1F1, 0x00);
    outb(0x1F2, sc);

    b_lba = lba & 0xFF;
    outb(0x1F3, b_lba);
    b_lba = (lba >> 8) & 0xFF;
    outb(0x1F4, b_lba);
    b_lba = (lba >> 16) & 0xFF;
    outb(0x1F5, b_lba);

    outb(0x1F7, 0x20);

    uint8_t sectors_left = sc;
    while (sectors_left)
    {
        ata_poll();
        ata_read_buffer(data);
        ata_delay();

        sectors_left--;
    }

    for (uint32_t i = 0; i < 30; i++)
    {
        //terminal_writehex((data[i*2+1] << 16) + data[i*2]);
    }
}

void ata_poll()
{
    uint8_t s = inb(0x1F7);

    while (!(((s & 0x08) && !(s & 0x80)) ||
        ((s & 0x01) || (s & 0x20))))
    {
        s = inb(0x1F7);
    }
}

void ata_delay()
{
    for (uint8_t i = 0; i < 4; i++)
        inb(0x80);
}
