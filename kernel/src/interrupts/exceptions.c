#include "interrupts/exceptions.h"

void ex_divide_error()
{
    terminal_writestring("Divide error occurred\n");
}
void ex_debug()
{
    terminal_writestring("Debug occurred\n");
}
void ex_nmi()
{
    terminal_writestring("NMI occurred\n");
}
void ex_breakpoint()
{
    terminal_writestring("Breakpoint occurred\n");
}
void ex_overflow()
{
    terminal_writestring("Overflow occurred\n");
}
void ex_bound_range()
{
    terminal_writestring("Bound range occurred\n");
}
void ex_invalid_opcode()
{
    terminal_writestring("Invalid opcode occurred\n");
}
void ex_device_not_available()
{
    terminal_writestring("Device not available occurred\n");
}
void ex_double_fault(uint32_t code)
{
    terminal_writestring("Double fault occurred\n");
}
void ex_coprocesser_segment_overrun()
{
    terminal_writestring("Coprocessor segment overrun occurred\n");
}
void ex_invalid_tss(uint32_t code)
{
    terminal_writestring("Invalid TSS occurred\n");
}
void ex_segment_not_present(uint32_t code)
{
    terminal_writestring("Segment not present occurred\n");
}
void ex_stack_segment_fault(uint32_t code)
{
    terminal_writestring("Stack segment fault occurred\n");
}
void ex_gpf(uint32_t code)
{
    terminal_writestring("GPF occurred\n");
}
void ex_page_fault(uint32_t code)
{
    terminal_writestring("Page fault occurred\n");
    terminal_writehex(get_cr2());
    asm("cli"); asm("hlt");
}
void ex_reserved()
{

}
