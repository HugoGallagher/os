FLAGS    equ 0x11
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot:
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

KERNEL_STACK_SIZE equ 16384

section .bss
align 16
stack_bottom:
    resb KERNEL_STACK_SIZE

section .text:
extern kernel_main
global _start
_start:
    mov esp, stack_bottom + KERNEL_STACK_SIZE
    call kernel_main
