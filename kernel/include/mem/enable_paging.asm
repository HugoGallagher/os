global pg_enable
pg_enable:
    ; set address of page directory
    mov eax, [esp+4]
    mov cr3, eax

    ; enable paging and enter protected mode
    mov ebx, cr0
    or  ebx, 0x80000001
    mov cr0, ebx
    ret
