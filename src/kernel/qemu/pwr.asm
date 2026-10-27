; pwr.asm
; shutdown for QEMU, in other hypervisors this may just hang the CPU, but QEMU will exit with code 0
[BITS 32]

section .text
global qemushutdown

qemushutdown:
    mov dx, 0xF4
    mov eax, 0x10
    out dx, eax              ; outl(0xF4, 0x10)

.hang:
    cli
    hlt
    jmp .hang