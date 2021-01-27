FLAGS    equ 0x3
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot.data
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

pd_boot:
    resb 4096
pt_boot1:
    resb 4096
pt_boot2:
    resb 4096

section .multiboot.text
extern kmain
extern _kernel_start
extern _kernel_end
global pd_boot
global pt_boot1
global pt_boot2
global _start
_start:
    push ebx

    ; the kernel has only 2 page directories, so it only has 7mb total
    mov edi, pt_boot1 - 0xC0000000
    mov esi, 0
    mov ecx, 1024 ; last 2 pages are for VGA buffer and pd_boot address
    mov edx, 0

    jmp l1

l1:
    cmp ecx, 0
    jg l2
    cmp ecx, 0
    jle l3

l2:
    add esi, 4096
    add edi, 4

    mov edx, esi
    or edx, 0x003
    mov [edi], edx

    add esi, 0x400000
    add edi, 0x400000

    mov edx, esi
    or edx, 0x003
    mov [edi], edx

    sub esi, 0x400000
    sub edi, 0x400000

    loop l1

l3:
    mov eax, pt_boot2 - 0xC0000000 + 1022*4
    mov ebx, 0xB8003 ; VGA buffer
    mov [eax], ebx
	mov eax, pt_boot2 - 0xC0000000 + 1023*4
    mov ebx, pd_boot ; pd_boot address
    mov [eax], ebx

    mov eax, pd_boot - 0xC0000000
    mov ebx, pt_boot1 - 0xC0000000 + 0x003
    mov [eax], ebx
    mov [eax+768*4], ebx
    mov ebx, pt_boot2 - 0xC0000000 + 0x003
    mov [eax+1*4], ebx
    mov [eax+769*4], ebx

    mov eax, pd_boot - 0xC0000000
    mov cr3, eax

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

    mov ecx, cr3
    mov cr3, ecx

	; mov eax, pt_boot1
	; cmp eax, 0xC0000000
	; jge l5

    pop ebx

    mov esp, stack_top

    push ebx

    call kmain

l5:
	cli
	hlt
