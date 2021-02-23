#include "interrupts/idt.h"

#include "interrupts/isrs.h"
#include "interrupts/pic.h"

#include "asm_io.h"

#include "interface/terminal.h"

void idt_handle(uint32_t a, uint32_t b, GeneralRegisters rs)
{

}
void idt_handle_error(uint32_t a, uint32_t b, GeneralRegisters rs)
{

}

void (*interrupt_handler_addresses[128])() =
{
    interrupt_handler_0,
	interrupt_handler_1,
	interrupt_handler_2,
	interrupt_handler_3,
	interrupt_handler_4,
	interrupt_handler_5,
	interrupt_handler_6,
	interrupt_handler_7,
	interrupt_handler_8,
	interrupt_handler_9,
	interrupt_handler_10,
	interrupt_handler_11,
	interrupt_handler_12,
	interrupt_handler_13,
	interrupt_handler_14,
	interrupt_handler_15,
	interrupt_handler_16,
	interrupt_handler_17,
	interrupt_handler_18,
	interrupt_handler_19,
	interrupt_handler_20,
	interrupt_handler_21,
	interrupt_handler_22,
	interrupt_handler_23,
	interrupt_handler_24,
	interrupt_handler_25,
	interrupt_handler_26,
	interrupt_handler_27,
	interrupt_handler_28,
	interrupt_handler_29,
	interrupt_handler_30,
	interrupt_handler_31,
	interrupt_handler_32,
	interrupt_handler_33,
	interrupt_handler_34,
	interrupt_handler_35,
	interrupt_handler_36,
	interrupt_handler_37,
	interrupt_handler_38,
	interrupt_handler_39,
	interrupt_handler_40,
	interrupt_handler_41,
	interrupt_handler_42,
	interrupt_handler_43,
	interrupt_handler_44,
	interrupt_handler_45,
	interrupt_handler_46,
	interrupt_handler_47,
	interrupt_handler_48,
	interrupt_handler_49,
	interrupt_handler_50,
	interrupt_handler_51,
	interrupt_handler_52,
	interrupt_handler_53,
	interrupt_handler_54,
	interrupt_handler_55,
	interrupt_handler_56,
	interrupt_handler_57,
	interrupt_handler_58,
	interrupt_handler_59,
	interrupt_handler_60,
	interrupt_handler_61,
	interrupt_handler_62,
	interrupt_handler_63,
	interrupt_handler_64,
};

IDTDescriptor* idt_create_descriptor(IDTHeader* ih, uint8_t interrupt_code, uint32_t offset, bool in_memory, uint8_t dpl, bool gate_size, uint16_t segment_selector)
{
    IDTDescriptor* cd = &(ih->descriptors[interrupt_code]); // current descriptor
    uint8_t capped_dpl = dpl;

    if (dpl > 3)
        capped_dpl = 3;
    cd->offset_high = offset >> 16;
    cd->offset_low = offset & 0x0000FFFF;

    cd->metadata = 0;
    cd->metadata = cd->metadata | (in_memory << 7);
    cd->metadata = cd->metadata | (capped_dpl << 5);
    cd->metadata = cd->metadata | (gate_size << 3);
    cd->metadata = cd->metadata | 0x6;

    cd->reserved = 0;

    cd->segment_selector = segment_selector;
}

void idt_fill_256(IDTHeader* ih)
{
    for (uint8_t i = 0; i < 64; i++)
    {
        idt_create_descriptor(ih, i, interrupt_handler_addresses[i], true, 3, true, 0x08);
    }
    idt_create_descriptor(ih, 64, interrupt_handler_addresses[64], true, 3, true, 0x08);
}
