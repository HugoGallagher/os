section .text

t_eip:
    resb 4

section .data

global asm_exit
asm_exit:
    mov eax, [esp]
    mov [t_eip], eax
    mov eax, 0
    int 0x7F
    jmp [t_eip]

global asm_yield
asm_yield:
    mov eax, [esp]
    mov [t_eip], eax
    mov eax, 1
    int 0x7F
    jmp [t_eip]

global asm_msg_send
asm_msg_send:
    mov eax, [esp]
    mov [t_eip], eax
    mov eax, 2
    mov edx, [esp+4]
    mov ebx, [esp+8]
    mov ecx, [esp+12]
    int 0x7F
    jmp [t_eip]

global asm_msg_get
asm_msg_get:
    mov eax, [esp]
    mov [t_eip], eax
    mov eax, 3
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int 0x7F
    jmp [t_eip]

global asm_msg_ack
asm_msg_ack:
    mov eax, [esp]
    mov [t_eip], eax
    mov eax, 4
    int 0x7F
    jmp [t_eip]

global asm_print
asm_print:
    mov eax, [esp]
    mov [t_eip], eax
    mov eax, 5
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int 0x7F
    jmp [t_eip]

global asm_print_hex
asm_print_hex:
    mov eax, [esp]
    mov [t_eip], eax
    mov eax, 6
    mov ebx, [esp+4]
    int 0x7F
    jmp [t_eip]
