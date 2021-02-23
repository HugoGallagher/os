SYS_CALL equ 0x40

section .text
t_ebp:
    resb 4

section .data

global exit
exit:
    mov [t_ebp], ebp

    mov eax, 0
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global yield
yield:
    mov [t_ebp], ebp

    mov eax, 1
    int SYS_CALL

    pop ebp
    ret

global msg_send
msg_send:
    mov [t_ebp], ebp

    mov eax, 2
    mov edx, [esp+4]
    mov ebx, [esp+8]
    mov ecx, [esp+12]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global msg_get
msg_get:
    mov [t_ebp], ebp

    mov eax, 3
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global msg_ack
msg_ack:
    mov [t_ebp], ebp

    mov eax, 4
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global print
print:
    mov [t_ebp], ebp

    mov eax, 5
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global print_hex
print_hex:
    mov [t_ebp], ebp

    mov eax, 6
    mov ebx, [esp+4]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global asm_loop
asm_loop:
    jmp $
