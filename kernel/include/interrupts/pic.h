#include <stdint.h>

#include "asm_io.h"

#define PIC1 0x20
#define PIC1_C 0x20
#define PIC1_D 0x21

#define PIC2 0xA0
#define PIC2_C 0xA0
#define PIC2_D 0xA1

#define PIC1_SI 0x20
#define PIC1_EI 0x27
#define PIC2_SI 0x28
#define PIC2_EI 0x2F

#define PIC_EOI 0x20

void pic_remap();

void pic_handle(uint8_t i);

void pic_ack(uint8_t i);
void pic_eoi(uint8_t i);

void pic_init_pit(uint16_t divider);

void pic_handle_irq0();
void pic_handle_irq1();
void pic_handle_irq2();
void pic_handle_irq3();
void pic_handle_irq4();
void pic_handle_irq5();
void pic_handle_irq6();
void pic_handle_irq7();
void pic_handle_irq8();
void pic_handle_irq9();
void pic_handle_irq10();
void pic_handle_irq11();
void pic_handle_irq12();
void pic_handle_irq13();
void pic_handle_irq14();
void pic_handle_irq15();
