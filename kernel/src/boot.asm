KERNEL_PAGES equ 0x1

FLAGS    equ 0x3
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot.data
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 0x1000
k_stack_bottom:
    resb 0x8000
k_stack_top:

pd_boot:
    resb 0x1000
pt_tasks:
    resb 0x1000
pt_buses:
    resb 0x1000
pt_boot:
    resb KERNEL_PAGES * 0x1000

section .multiboot.text
extern kmain
extern _kernel_start
extern _kernel_end
global _start
_start:
    push ebx

    mov edi, pt_boot - 0xC0000000
    mov esi, 0
    mov ecx, KERNEL_PAGES * 1024
    mov edx, 0

    jmp l1

l1:
    cmp ecx, 0
    jg l2
    cmp ecx, 0
    jle l3

l2:
    ; esi is the physical address
    ; edi is the page table entry

    mov edx, esi
    or edx, 0x103
    mov [edi], edx

    add esi, 4096
    add edi, 4

    dec ecx
    jnz l1

l3:
    mov eax, pd_boot - 0xC0000000
    mov ebx, pt_boot - 0xC0000000 + 0x003
    mov ecx, KERNEL_PAGES

l4:
    mov [eax], ebx
    add eax, 768*4
    mov [eax], ebx
    sub eax, 768*4

    add eax, 4
    add ebx, 4096

    dec ecx
    cmp ecx, 0
    je l5

    jmp l4

l5:
    ; recursively map page directory to last pde
    mov eax, pd_boot - 0xC0000000
    mov ebx, pd_boot - 0xC0000000 + 0x003
    mov [eax+1023*4], ebx

    ; 2nd-last pde is for task information
    mov eax, pd_boot - 0xC0000000
    mov ebx, pt_tasks - 0xC0000000 + 0x003
    mov [eax+1022*4], ebx

    ; 3rd-last pde is for message buses
    mov eax, pd_boot - 0xC0000000
    mov ebx, pt_buses - 0xC0000000 + 0x007
    mov [eax+1021*4], ebx

    mov eax, pd_boot - 0xC0000000
    mov cr3, eax

    mov eax, cr4
    or eax, 0x00000080
    mov cr4, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    lea eax, [unmap_identity_pages]
    jmp near eax

section .text

unmap_identity_pages:
    mov eax, pd_boot
    mov ecx, KERNEL_PAGES
    jmp unmap

unmap:
    mov ebx, [eax]
    mov ebx, 0

    add eax, 4

    dec ecx
    cmp ecx, 0
    je unmap

    jmp enter_kernel

enter_kernel:
    mov eax, cr3
    mov cr3, eax

    pop ebx

    mov esp, k_stack_top

    push ebx

    call kmain
