extern idt_handle
extern idt_handle_error

%macro no_error_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    pusha
    push dword 0
    push %1
    call idt_handle
    popa
    iret
%endmacro

%macro error_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    pusha
    popa
    iret
%endmacro

%macro irq_interrupt_handler 2
extern pic_handle_irq%2
global interrupt_handler_%1
interrupt_handler_%1:
    pusha
    call pic_handle_irq%2
    popa
    iret
%endmacro

global interrupt_handler_32
extern tm_is_multitasking
extern tm_get_task_stack_base
extern tm_preempt_pl0
extern tm_preempt_pl3
interrupt_handler_32:
    cli

    push eax
    mov eax, [esp+8]
    test eax, 0x03
    jne call_preemption_function_3

    jmp call_preemption_function_0

call_preemption_function_0:
    add esp, 4
    call tm_is_multitasking
    cmp eax, 0
    je return_to_pl0

    pusha
    call tm_preempt_pl0
    popa
    iret

call_preemption_function_3:
    pop eax
    pusha
    call tm_preempt_pl3
    popa
    iret

return_to_pl0:
    mov ax, 0x20
    mov dx, 0x20
    out dx, ax
    sti
    iret

global interrupt_handler_64
extern sys_call
    interrupt_handler_64:
    pusha
    call sys_call
    popa
    iret

extern ex_divide_error
extern ex_debug
extern ex_nmi
extern ex_breakpoint
extern ex_overflow
extern ex_bound_range
extern ex_invalid_opcode
extern ex_device_not_available
extern ex_double_fault
extern ex_coprocesser_segment_overrun
extern ex_invalid_tss
extern ex_segment_not_present
extern ex_stack_segment_fault
extern ex_gpf
extern ex_page_fault
extern ex_reserved

global interrupt_handler_0
interrupt_handler_0:
    pusha
    call ex_divide_error
    popa
    iret

global interrupt_handler_1
interrupt_handler_1:
    pusha
    call ex_debug
    popa
    iret

global interrupt_handler_2
interrupt_handler_2:
    pusha
    call ex_nmi
    popa
    iret

global interrupt_handler_3
interrupt_handler_3:
    pusha
    call ex_breakpoint
    popa
    iret

global interrupt_handler_4
interrupt_handler_4:
    pusha
    call ex_overflow
    popa
    iret

global interrupt_handler_5
interrupt_handler_5:
    pusha
    call ex_bound_range
    popa
    iret

global interrupt_handler_6
interrupt_handler_6:
    pusha
    call ex_invalid_opcode
    popa
    iret

global interrupt_handler_7
interrupt_handler_7:
    pusha
    call ex_device_not_available
    popa
    iret

global interrupt_handler_8
interrupt_handler_8:
    pusha
    call ex_double_fault
    popa
    iret

global interrupt_handler_9
interrupt_handler_9:
    pusha
    call ex_coprocesser_segment_overrun
    popa
    iret

global interrupt_handler_10
interrupt_handler_10:
    pusha
    call ex_invalid_tss
    popa
    iret

global interrupt_handler_11
interrupt_handler_11:
    pusha
    call ex_segment_not_present
    popa
    iret

global interrupt_handler_12
interrupt_handler_12:
    pusha
    call ex_stack_segment_fault
    popa
    iret

global interrupt_handler_13
interrupt_handler_13:
    pusha
    call ex_gpf
    popa
    iret

global interrupt_handler_14
interrupt_handler_14:
    pusha
    call ex_page_fault
    popa
    iret

global interrupt_handler_15
interrupt_handler_15:
    pusha
    call ex_reserved
    popa
    iret

irq_interrupt_handler 33, 1
irq_interrupt_handler 34, 2
irq_interrupt_handler 35, 3
irq_interrupt_handler 36, 4
irq_interrupt_handler 37, 5
irq_interrupt_handler 38, 6
irq_interrupt_handler 39, 7
irq_interrupt_handler 40, 8
irq_interrupt_handler 41, 9
irq_interrupt_handler 42, 10
irq_interrupt_handler 43, 11
irq_interrupt_handler 44, 12
irq_interrupt_handler 45, 13
irq_interrupt_handler 46, 14
irq_interrupt_handler 47, 15

error_interrupt_handler 16

no_error_interrupt_handler 17
no_error_interrupt_handler 18
no_error_interrupt_handler 19
no_error_interrupt_handler 20
no_error_interrupt_handler 21
no_error_interrupt_handler 22
no_error_interrupt_handler 23
no_error_interrupt_handler 24
no_error_interrupt_handler 25
no_error_interrupt_handler 26
no_error_interrupt_handler 27
no_error_interrupt_handler 28
no_error_interrupt_handler 29
no_error_interrupt_handler 30
no_error_interrupt_handler 31

no_error_interrupt_handler 48
no_error_interrupt_handler 49
no_error_interrupt_handler 50
no_error_interrupt_handler 51
no_error_interrupt_handler 52
no_error_interrupt_handler 53
no_error_interrupt_handler 54
no_error_interrupt_handler 55
no_error_interrupt_handler 56
no_error_interrupt_handler 57
no_error_interrupt_handler 58
no_error_interrupt_handler 59
no_error_interrupt_handler 60
no_error_interrupt_handler 61
no_error_interrupt_handler 62
no_error_interrupt_handler 63
