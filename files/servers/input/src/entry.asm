extern main

section .text
    call main
    mov eax, 0
    int 0x7F
