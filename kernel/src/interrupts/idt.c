#include "interrupts/idt.h"

#include "interrupts/isrs.h"
#include "interrupts/pic.h"

#include "asm_io.h"

#include "interface/terminal.h"

void idt_handle(uint32_t a, uint32_t b, GeneralRegisters rs)
{
    terminal_writestring("Exception occurred:\n");
    terminal_writehex(a);
    asm("cli");
    asm("hlt");
}
void idt_handle_error(uint32_t a, uint32_t b, GeneralRegisters rs)
{
    terminal_writestring("Exception occurred:\n");
    terminal_writehex(a);
    terminal_writestring("Error code:\n");
    terminal_writehex(b);
    asm("cli");
    asm("hlt");
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
	interrupt_handler_65,
	interrupt_handler_66,
	interrupt_handler_67,
	interrupt_handler_68,
	interrupt_handler_69,
	interrupt_handler_70,
	interrupt_handler_71,
	interrupt_handler_72,
	interrupt_handler_73,
	interrupt_handler_74,
	interrupt_handler_75,
	interrupt_handler_76,
	interrupt_handler_77,
	interrupt_handler_78,
	interrupt_handler_79,
	interrupt_handler_80,
	interrupt_handler_81,
	interrupt_handler_82,
	interrupt_handler_83,
	interrupt_handler_84,
	interrupt_handler_85,
	interrupt_handler_86,
	interrupt_handler_87,
	interrupt_handler_88,
	interrupt_handler_89,
	interrupt_handler_90,
	interrupt_handler_91,
	interrupt_handler_92,
	interrupt_handler_93,
	interrupt_handler_94,
	interrupt_handler_95,
	interrupt_handler_96,
	interrupt_handler_97,
	interrupt_handler_98,
	interrupt_handler_99,
	interrupt_handler_100,
	interrupt_handler_101,
	interrupt_handler_102,
	interrupt_handler_103,
	interrupt_handler_104,
	interrupt_handler_105,
	interrupt_handler_106,
	interrupt_handler_107,
	interrupt_handler_108,
	interrupt_handler_109,
	interrupt_handler_110,
	interrupt_handler_111,
	interrupt_handler_112,
	interrupt_handler_113,
	interrupt_handler_114,
	interrupt_handler_115,
	interrupt_handler_116,
	interrupt_handler_117,
	interrupt_handler_118,
	interrupt_handler_119,
	interrupt_handler_120,
	interrupt_handler_121,
	interrupt_handler_122,
	interrupt_handler_123,
	interrupt_handler_124,
	interrupt_handler_125,
	interrupt_handler_126,
	interrupt_handler_127,
	interrupt_handler_128,
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
    for (uint8_t i = 0; i < 127; i++)
    {
        idt_create_descriptor(ih, i, interrupt_handler_addresses[i], true, 3, true, 0x0008);
    }
    idt_create_descriptor(ih, 127, interrupt_handler_addresses[127], true, 3, true, 0x0008);
}
