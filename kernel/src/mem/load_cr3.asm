global load_cr3
load_cr3:
    mov eax, [esp+4]
    mov cr3, eax

    ret
