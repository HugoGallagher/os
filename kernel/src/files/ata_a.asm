extern terminal_writehex

global ata_read_buffer
global ata_poll
global ata_identify

ata_read_buffer:
    mov dx, 0x1F0
    mov edi, [esp+4]
    mov ecx, 256
    rep insw

    ret

ata_identify:
    mov eax, 0

    mov al, 0x00
    mov dx, 0x1F6
    out dx, al

    mov al, 0x00

    mov dx, 0x1F2
    out dx, al
    mov dx, 0x1F3
    out dx, al
    mov dx, 0x1F4
    out dx, al
    mov dx, 0x1F5
    out dx, al

    mov al, 0xEC
    mov dx, 0x1F7
    out dx, al

    in ax, dx
    cmp ax, 0
    je drive_error

    jmp poll_status

poll_status:
    mov dx, 0x1F7
    in ax, dx
    test ax, 0x80
    jg poll_status

    jmp check_nonzero_lba

check_nonzero_lba:
    mov bx, 0

    mov dx, 0x1F4

    in ax, dx
    or bx, ax
    inc edx

    in ax, dx
    or bx, ax
    inc edx

    cmp bx, 0
    je drive_error

    jmp poll_status_final

poll_status_final:
    mov dx, 0x1F7
    in ax, dx
    test ax, 0x09
    je poll_status_final

    mov dx, 0x1F0
    mov edi, [esp+4]
    mov ecx, 256
    rep insw

    ret

drive_error:
    mov eax, 0x1234
    push eax
    call terminal_writehex
    pop eax

    ret
