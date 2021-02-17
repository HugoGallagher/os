global asm_msg_send
asm_msg_send:
    mov eax, 2
    mov edx, [esp+4]
    mov ebx, [esp+8]
    mov ecx, [esp+12]
    int 0x7F

    ret

global asm_msg_get
asm_msg_get:
    mov eax, 3
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int 0x7F

    ret

global asm_msg_ack
asm_msg_ack:
    mov eax, 4
    int 0x7F

    ret

global asm_print
asm_print:
    mov eax, 5
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int 0x7F

    ret