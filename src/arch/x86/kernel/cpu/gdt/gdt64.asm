;
; gdt.asm - Global Descriptor Table setup
;

bits 32

global gdt_init
global gdt
global gdt_descriptor

gdt_init:
;
; We are in 32-bit protected mode, but also init the 64-bit segments so we can jump to long mode
; 

    cli

    lgdt [gdt_descriptor]

    ; reload segment registers (r0)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; reload CS with a far jump
    jmp 0x08:.flush_cs

.flush_cs:
    ret

align 8
gdt:
    ; --- null segment ---
    dq 0x0000000000000000 ; 0x00 null

    ; --- 64-bit KERNEL segments ---
    dq 0x00AF9A000000FFFF ; 0x08 r0 64-bit code 
    dq 0x00CF92000000FFFF ; 0x10 r0 data 

    ; --- user segments ---
    dq 0x00CFF2000000FFFF ; 0x18 r3 data
    dq 0x00AFFA000000FFFF ; 0x20 r3 64-bit code

    ; --- TSS ---
    ; initialized at runtime
    tss_descriptor:
        dq 0x0000000000000000 ; 0x28 TSS (low 8 B)
        dq 0x0000000000000000 ; 0x30 TSS (high 8 B)

    ; 32-bit BOOT segment (kernel)
    dq 0x00CF9A000000FFFF ; 0x38 r0 32-bit code (BOOT ONLY)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dd gdt