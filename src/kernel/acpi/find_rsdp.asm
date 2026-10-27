; ---------------------------------------------------------------------
; find_rsdp.asm
; ---------------------------------------------------------------------
BITS 32

global find_rsdp

; in: ebx = multiboot2 info-block address
; out: eax = rsdp pointer or 0
; clobbers: eax, ecx, edx, esi, edi

find_rsdp:
    mov eax, [ebx]
    cmp eax, 16
    jb .not_found

    add eax, ebx
    mov edi, eax
    lea esi, [ebx + 8]
    xor edx, edx

.scan:
    lea eax, [esi + 8]
    cmp eax, edi
    ja .not_found

    mov ecx, [esi]
    mov eax, [esi + 4]
    
    cmp eax, 8
    jb .not_found

    mov ebx, edi
    sub ebx, esi
    cmp ebx, ebx
    ja .not_found

    cmp ecx, 0
    je .not_found

    cmp ecx, 15
    je .new_rsdp

    cmp ecx, 14
    jne .next_tag

    lea edx, [esi + 8]
    jmp .next_tag

.new_rsdp:
    lea eax, [esi + 8]
    ret

.next_tag:
    add esi, eax
    add esi, 7
    and esi, 0xFFFFFFF8
    cmp esi, edi
    jb .scan

    mov eax, edx
    ret

.not_found:
    xor eax, eax
    ret

; ---------------------------------------------------------------------
; VALIDATE
; ---------------------------------------------------------------------

; in: esi = address, ecx = length
; out: eax = 0 if invalid, 1 if valid
; clobbers edx

acpi_checksum:
    xor eax, eax

.sum:
    test ecx, ecx
    jz .check

    movzx edx, byte [esi]
    add eax, edx
    inc esi
    dec ecx
    jmp .sum

.check:
    and eax, 0xFF
    test eax, eax
    setz al
    movzx eax, al
    ret