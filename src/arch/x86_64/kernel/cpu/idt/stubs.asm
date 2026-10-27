; stubs.asm - ISR stub generation

bits 64
default rel

section .text
extern isr_handler


; No error code
%macro ISR_NOERR 1
global isr%1
isr%1:
    push qword 0
    push qword %1
    jmp isr_common_stub
%endmacro

; Exceptions with error code pushed by CPU
%macro ISR_ERR 1
global isr%1
isr%1:
    push qword %1
    jmp isr_common_stub
%endmacro

; Assign vectors
%assign i 0
%rep 256
    %if i = 8 || i = 10 || i = 11 || i = 12 || i = 13 || i = 14 || i = 17 || i = 21 || i = 29 || i = 30
        ISR_ERR i
    %else
        ISR_NOERR i
    %endif
    %assign i i + 1
%endrep


isr_common:
    cld

    ; i sometimes wish there was pusha in x86-64
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; pass ptr to saved frame to C
    mov rdi, rsp
    mov rbx, rsp
    and rsp, -16
    call isr_handler
    mov rsp, rbx

    ; and popa
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax


    ; discard evctor and errorcode
    add rsp, 16
    iretq

section .rodata
align 8
global isr_stub_table

isr_stub_table:
%assign i 0
%rep 256
    dq isr_%+i
    %assign i i + 1
%endrep