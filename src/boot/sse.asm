; sse.asm - Enabling SSE/SSE2

bits 64

global enable_sse

enable_sse:
    cli

    ; Enable SSE support in CR0
    ; CR0.MP = 1
    ; CR0.EM = 0
    mov rax, cr0
    and rax, ~(1 << 2)
    or rax, (1 << 1)
    mov cr0, rax

    ; Enable SSE & SSE exception support in CR4
    ; CR4.OSFXSR = 1
    ; CR4.OSXMMEXCPT = 1
    mov rax, cr4
    or rax, (1 << 9) | (1 << 10)
    mov cr4, rax

    fninit ; Init x87 FPU

    ; Init MXCSR
    ; mask floating-point exceptions
    ; Default MXCSR = 0x1F80
    sub rsp, 8
    mov dword [rsp], 0x1F80
    ldmxcsr [rsp]
    add rsp, 8