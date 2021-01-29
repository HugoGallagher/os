global tss_reload
tss_reload:
    mov ax, 0x28
    ltr ax
    ret
