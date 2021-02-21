extern main

section .text
    call main
    mov eax, 1
    int 0x7F
