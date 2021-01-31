FLAGS    equ 0x3
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot.data
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 4096
k_stack_bottom:
    resb 16384
k_stack_top:

pd_boot:
    resb 4096
pt_boot1:
    resb 4096
pt_boot2:
    resb 4096
pt_boot3:
    resb 4096

section .multiboot.text
extern kmain
extern _kernel_start
extern _kernel_end
global _start
_start:
    push ebx

    mov edi, pt_boot1 - 0xC0000000
    mov esi, 0
    mov ecx, 3072
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
    mov eax, pt_boot3 - 0xC0000000 + 1023*4
    mov ebx, 0xB8003
    mov [eax], ebx

    mov eax, pd_boot - 0xC0000000
    mov ebx, pt_boot1 - 0xC0000000 + 0x003
    mov [eax], ebx
    mov [eax+768*4], ebx
    mov ebx, pt_boot2 - 0xC0000000 + 0x003
    mov [eax+1*4], ebx
    mov [eax+769*4], ebx
    mov ebx, pt_boot3 - 0xC0000000 + 0x003
    mov [eax+2*4], ebx
    mov [eax+770*4], ebx

    mov ebx, pd_boot - 0xC0000000 + 0x003
    mov [eax+1023*4], ebx

    mov eax, pd_boot - 0xC0000000
    mov cr3, eax

    mov eax, cr4
    or eax, 0x00000080
    mov cr4, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    lea eax, [enter_kernel]
    jmp near eax

section .text

enter_kernel:
    mov eax, [pd_boot+0*4]
    mov eax, 0
	mov eax, [pd_boot+1*4]
    mov eax, 0
	mov eax, [pd_boot+2*4]
    mov eax, 0

    mov ecx, cr3
    mov cr3, ecx

    pop ebx

    mov esp, k_stack_top

    push ebx

    call kmain
