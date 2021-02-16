global print
print:
    mov eax, 4
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int 0x7F

    ret
