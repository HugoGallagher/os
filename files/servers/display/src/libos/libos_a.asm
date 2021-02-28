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

global create_process
create_process:
    mov [t_ebp], ebp

    mov eax, 2
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global get_server_id
get_server_id:
    mov [t_ebp], ebp

;    mov eax, 3
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global msg_send
msg_send:
    mov [t_ebp], ebp

    mov eax, 4
    mov edx, [esp+4]
    mov ebx, [esp+8]
    mov ecx, [esp+12]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global msg_get
msg_get:
    mov [t_ebp], ebp

    mov eax, 5
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global msg_ack
msg_ack:
    mov [t_ebp], ebp

    mov eax, 6
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global d_copy_buffer
d_copy_buffer:
    mov [t_ebp], ebp

    mov eax, 7
    mov ebx, [esp+4]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global d_put_pixel
d_put_pixel:
    mov [t_ebp], ebp

    mov eax, 8
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    mov edx, [esp+12]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global d_put_pixels
d_put_pixels:
    mov [t_ebp], ebp

    mov eax, 9
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    mov edx, [esp+12]
    mov esi, [esp+16]
    int SYS_CALL

    mov ebp, [t_ebp]
    ret

global asm_loop
asm_loop:
    jmp $
