section .data
t_eax:
    resb 4
t_ebx:
    resb 4

section .text
global enter_usr_pl3
enter_usr_pl3:
    pop eax

    popa
    popf

    mov [t_eax], eax
    mov [t_ebx], ebx

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    pop eax
    pop ebx

    push 0x23
    push eax
    pushf
    push 0x1B
    push ebx

    mov eax, [t_eax]
    mov ebx, [t_ebx]

    iret

global enter_usr_pl0
extern tm_get_task_stack_base
enter_usr_pl0:
    pop eax

    popa
    popf

    mov [t_eax], eax
    mov [t_ebx], ebx

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    pop eax
    pop ebx

    push 0x10
    push eax
    pushf
    push 0x18
    push ebx

    call tm_get_task_stack_base
    add eax, 4092
    mov esp, eax

    mov eax, [t_eax]
    mov ebx, [t_ebx]

    iret

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
