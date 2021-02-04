global inb
global outb

global inl
global outl

ainb:
    mov dx, [esp+4]
    in al, dx
    ret

aoutb:
    mov al, [esp+8]
    mov dx, [esp+4]
    out dx, al
    ret

ainw:
    mov dx, [esp+4]
    in eax, dx
    ret

aoutw:
    mov eax, [esp+8]
    mov dx, [esp+4]
    out dx, eax
    ret

ainl:
    mov dx, [esp+4]
    in eax, dx
    ret

aoutl:
    mov eax, [esp+8]
    mov dx, [esp+4]
    out dx, eax
    ret
