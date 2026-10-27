; pwr.asm
; shutdown for QEMU, in other hypervisors this may just hang the CPU, but QEMU will exit with code 0
; x86_64 version! dont use this
[BITS 64]

section .text
global qemushutdownx64

qemushutdownx64:
    mov dx, 0xF4
    mov eax, 0x10
    out dx, eax

.hang:
    cli
    hlt
    jmp .hang