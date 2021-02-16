#include "interrupts/exceptions.h"

void ex_divide_error()
{

}
void ex_debug()
{

}
void ex_nmi()
{

}
void ex_breakpoint()
{

}
void ex_overflow()
{

}
void ex_bound_range()
{

}
void ex_invalid_opcode()
{

}
void ex_device_not_available()
{

}
void ex_double_fault(uint32_t code)
{

}
void ex_coprocesser_segment_overrun()
{

}
void ex_invalid_tss(uint32_t code)
{

}
void ex_segment_not_present(uint32_t code)
{

}
void ex_stack_segment_fault(uint32_t code)
{

}
void ex_gpf(uint32_t code)
{

}
void ex_page_fault(uint32_t code)
{
    terminal_writestring("Page fault occurred\n");
    terminal_writehex(get_cr2());
}
void ex_reserved()
{

}
