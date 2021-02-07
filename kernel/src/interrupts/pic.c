#include "interrupts/pic.h"

#include "interface/terminal.h"
#include "interface/cli.h"

void pic_remap()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

    outb(PIC1_D, 0b11111101);
    outb(PIC2_D, 0b00111111);
}

void pic_handle(uint8_t i)
{
    terminal_writehex(i);

    switch(i)
    {
        case 1:
            pic_handle_irq0();
            break;
        case 2:
            pic_handle_irq1();
            break;
        default:
            pic_ack(i);
    }

    pic_ack(i);

    //return sc;
}

void pic_ack(uint8_t i)
{
    pic_eoi(i);
}

void pic_eoi(uint8_t i)
{
    outb(PIC1_C, PIC_EOI);
    if (i > 7)
        outb(PIC2_C, PIC_EOI);
}

void pic_init_pit(uint16_t divider)
{
    outb(0x43, 0b00110110);
    outb(0x40, divider & 0xFF);
    outb(0x40, divider >> 8);
}

void pic_handle_irq0()
{
    //terminal_writestring("Ticked!\n");

    pic_ack(0);
}
void pic_handle_irq1()
{
    uint8_t sc = inb(0x60);

    if (sc & 0x80) {} // this should track keys like shift
    //else { cli_send_char(sc); }

    pic_ack(1);
}
void pic_handle_irq2()
{
    pic_ack(2);
}
void pic_handle_irq3()
{
    pic_ack(3);
}
void pic_handle_irq4()
{
    pic_ack(4);
}
void pic_handle_irq5()
{
    pic_ack(5);
}
void pic_handle_irq6()
{
    pic_ack(6);
}
void pic_handle_irq7()
{
    pic_ack(7);
}
void pic_handle_irq8()
{
    pic_ack(8);
}
void pic_handle_irq9()
{
    pic_ack(9);
}
void pic_handle_irq10()
{
    pic_ack(10);
}
void pic_handle_irq11()
{
    pic_ack(11);
}
void pic_handle_irq12()
{
    pic_ack(12);
}
void pic_handle_irq13()
{
    pic_ack(13);
}
void pic_handle_irq14()
{
    terminal_writehex("a");

    inb(0x1F7);

    pic_ack(14);
}
void pic_handle_irq15()
{
    terminal_writehex("a");

    inb(0x1F7);

    pic_ack(15);
}
