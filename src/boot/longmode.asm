; longmode.asm - Long Mode jump for BirdOS

bits 32

global long_mode_init
extern kernel_load
extern pml4_table
extern stack_top

long_mode_init:
    cli

    ; Eanble PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    
    ; Load pml4 physical address
    mov eax, pml4_table
    mov cr3, eax
    
    ; enable Long mode in EFER
    mov ecx, 0xC0000080     ; IA32_EFER MSR
    rdmsr
    or eax, 1 << 8          ; LME = 1
    wrmsr
    
    ; Enable pagign
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    
    ; far jump into 64bit code segment
    jmp 0x08:long_mode_entry

; ----- 64-bit long mode -----
    
bits 64

extern enable_sse

extern mb_magic
extern mb_info

long_mode_entry:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    call enable_sse

    ; 64-bit stack init
    lea rsp, [rel stack_top]
    and rsp, -16

    xor ebp, ebp

    mov edi, [rel mb_magic]
    mov esi, [rel mb_info]
    ; Now that we are in long mode, we can execute 64-bit code,
    ; we have 64-bit registers (rax instead of eax, r8 through r15 etc.) and
    ; let's continue now 64-bit execution :D

    call kernel_load


lm_hang:
    cli
    hlt
    jmp lm_hang