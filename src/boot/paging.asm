; paging.asm

bits 32

global paging_init
global pml4_table

section .text

paging_init:
    ; clear all tables
    cld
    xor eax, eax

    mov edi, pml4_table
    mov ecx, 3072
    rep stosd

    ; PML4[0] -> PDPT
    mov eax, pdpt_table
    or eax, 0x03
    mov [pml4_table], eax

    ; PDPT[0] -> page directorry
    mov eax, pd_table
    or eax, 0x03
    mov [pdpt_table], eax

    ; build 512 x 2 MiB identity mapping
    xor ecx, ecx

.fill_pd:
    mov eax, ecx
    shl eax, 21
    or eax, 0x83
    mov [pd_table + ecx * 8], eax

    ; upper 32bits of pde = 0
    mov dword [pd_table + ecx * 8 + 4], 0

    inc ecx
    cmp ecx, 512
    jne .fill_pd

    ret



section .bss

align 4096
pml4_table:
    resb 4096

align 4096
pdpt_table:
    resb 4096

align 4096
pd_table:
    resb 4096