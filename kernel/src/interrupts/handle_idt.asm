extern terminal_writehex

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
    push %1
    call idt_handle_error
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

global interrupt_handler_127
extern sys_call
    interrupt_handler_127:
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

irq_interrupt_handler 32, 0
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
no_error_interrupt_handler 64
no_error_interrupt_handler 65
no_error_interrupt_handler 66
no_error_interrupt_handler 67
no_error_interrupt_handler 68
no_error_interrupt_handler 69
no_error_interrupt_handler 70
no_error_interrupt_handler 71
no_error_interrupt_handler 72
no_error_interrupt_handler 73
no_error_interrupt_handler 74
no_error_interrupt_handler 75
no_error_interrupt_handler 76
no_error_interrupt_handler 77
no_error_interrupt_handler 78
no_error_interrupt_handler 79
no_error_interrupt_handler 80
no_error_interrupt_handler 81
no_error_interrupt_handler 82
no_error_interrupt_handler 83
no_error_interrupt_handler 84
no_error_interrupt_handler 85
no_error_interrupt_handler 86
no_error_interrupt_handler 87
no_error_interrupt_handler 88
no_error_interrupt_handler 89
no_error_interrupt_handler 90
no_error_interrupt_handler 91
no_error_interrupt_handler 92
no_error_interrupt_handler 93
no_error_interrupt_handler 94
no_error_interrupt_handler 95
no_error_interrupt_handler 96
no_error_interrupt_handler 97
no_error_interrupt_handler 98
no_error_interrupt_handler 99
no_error_interrupt_handler 100
no_error_interrupt_handler 101
no_error_interrupt_handler 102
no_error_interrupt_handler 103
no_error_interrupt_handler 104
no_error_interrupt_handler 105
no_error_interrupt_handler 106
no_error_interrupt_handler 107
no_error_interrupt_handler 108
no_error_interrupt_handler 109
no_error_interrupt_handler 110
no_error_interrupt_handler 111
no_error_interrupt_handler 112
no_error_interrupt_handler 113
no_error_interrupt_handler 114
no_error_interrupt_handler 115
no_error_interrupt_handler 116
no_error_interrupt_handler 117
no_error_interrupt_handler 118
no_error_interrupt_handler 119
no_error_interrupt_handler 120
no_error_interrupt_handler 121
no_error_interrupt_handler 122
no_error_interrupt_handler 123
no_error_interrupt_handler 124
no_error_interrupt_handler 125
no_error_interrupt_handler 126
;no_error_interrupt_handler 127
