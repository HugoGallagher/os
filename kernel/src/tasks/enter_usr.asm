section .data
t_eax:
    resb 4
t_ebx:
    resb 4

section .text
global enter_usr
enter_usr:
    pop eax

    popa
    popf

    mov [t_eax], eax
    mov [t_ebx], ebx

    pop eax
    pop ebx

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23
    push eax
    pushf
    push 0x1B
    push ebx

    mov eax, [t_eax]
    mov ebx, [t_ebx]

    iret

global set_usr_srs
set_usr_srs:
    mov ax, 0x23
    mov dx, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

global exit_usr
exit_usr:
    pusha
    pushf

    call get_eip
    push eax

    ret

global get_eip;
get_eip:
    mov eax, [esp]
    ret

global get_flags
get_flags:
    pushf
    pop eax
    ret

global get_esp
get_esp:
    mov eax, esp
    ret

global get_cr2
get_cr2:
    mov eax, cr2
    ret
