global gdt_load
global gdt_reload_cs

; from osdev.org

gdtr dw 0
     dd 0

gdt_load:
    cli
    mov ax, [esp+4]
    mov [gdtr], ax
    mov eax, [esp+6]
    mov [gdtr+2], eax
    lgdt[gdtr]
    ret

gdt_reload_cs:
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:reload_cs

reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
