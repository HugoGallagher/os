extern main

extern asm_yield

section .text
    mov esp, 0xBFFFFFFB
    mov eax, 6
    mov ebx, 0x200
    int 0x7F

    call asm_yield
;    mov eax, 1
;    int 0x7F

    mov eax, 6
    mov ebx, 0x300
    int 0x7F

;    call main
    mov eax, 0
    int 0x7F
    jmp $
