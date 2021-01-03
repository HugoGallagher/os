global gdt_load
global gdt_enable_protected_mode

; from osdev.org

gdtr dw 0
     dd 0

gdt_load:
    mov eax, [esp+4]
    mov [gdtr+2], eax
    mov ax, [esp+8]
    mov [gdtr], ax
    cli
    lgdt[gdtr]
    sti
    ret

gdt_enable_protected_mode:
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

global halt
halt:
    cli
    hlt
