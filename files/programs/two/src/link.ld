OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x0;

    .text ALIGN(4):
    {
        build/entry.o(.text)
        *(.text)
    }
    
    .data ALIGN(4):
    {
        *(.data)
    }
    
    .rodata ALIGN(4):
    {
        *(rotdata*)
    }
}

