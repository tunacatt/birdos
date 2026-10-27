ENTRY(_start)

SECTIONS
{
    . = 1M;
    .trampoline : { *(.trampoline) }

    . += 0xFFFFFFFF80000000;
    .text ALIGN(4K) : AT(ADDR(.text) - 0xFFFFFFFF80000000) { *(.text) }

    .multiboot ALIGN(4) :
    {
        *(.multiboot)
    }

    .text ALIGN(4K) :
    {
        *(.text)
    }

    .rodata ALIGN(4K) :
    {
        *(.rodata*)
    }

    .data ALIGN(4K) :
    {
        *(.data)
    }

    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss)
    }
}