;
; BirdOS boot.asm - multiboot entry point
;
MBALIGN  equ 1<<0
MEMINFO  equ 1<<1
FLAGS    equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
global mb_magic
global mb_info
mb_magic: resd 1
mb_info: resd 1

global stack_top
alignb 16
stack_bottom:
    resb 16384
stack_top:

section .text
bits 32
global _start
extern long_mode_init
extern paging_init
extern gdt_init
extern cpu_check
_start:
    cli
    cld
    mov esp, stack_top
    and esp, -16
    mov edi, eax
    mov esi, ebx
    mov [mb_magic], eax
    mov [mb_info], ebx
    call cpu_check
    call gdt_init
    call paging_init
    call long_mode_init
    cli

boot_hang:
    cli
    hlt
    jmp boot_hang