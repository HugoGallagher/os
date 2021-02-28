#include "interrupts/exceptions.h"

#include "tasks/tasks.h"

void ex_divide_error()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Divide error occurred\n");
}
void ex_debug()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Debug occurred\n");
}
void ex_nmi()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("NMI occurred\n");
}
void ex_breakpoint()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Breakpoint occurred\n");
}
void ex_overflow()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Overflow occurred\n");
}
void ex_bound_range()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Bound range occurred\n");
}
void ex_invalid_opcode()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Invalid opcode occurred\n");
}
void ex_device_not_available()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Device not available occurred\n");
}
void ex_double_fault(uint32_t code)
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Double fault occurred\n");
}
void ex_coprocesser_segment_overrun()
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Coprocessor segment overrun occurred\n");
}
void ex_invalid_tss(uint32_t code)
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Invalid TSS occurred\n");
}
void ex_segment_not_present(uint32_t code)
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Segment not present occurred\n");
}
void ex_stack_segment_fault(uint32_t code)
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Stack segment fault occurred\n");
}
void ex_gpf(uint32_t code)
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("GPF occurred\n");
    terminal_hex(code);
    asm("cli"); asm("hlt");
}
void ex_page_fault(uint32_t code)
{
    if (tm_is_multitasking())
        tm_delete_active_task();

    terminal_writestring("Page fault occurred\n");
    terminal_hex(get_cr2());
    asm("cli"); asm("hlt");
}
void ex_reserved()
{

}
