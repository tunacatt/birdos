;
; idt.asm - Interrupt Descriptor Table setup for 32-bit Protected mode
;
; Works only WITH 32-BIT MODE AND GDT
; I am keeping this so later I can still have this when making 32-bit compat

bits 32

global idt_init
global cpu_interrupt
extern isr_handler
extern irq_handler

; ----- Stub generators -----
%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push dword 0
    push dword %1 
    jmp isr_common_stub
%endmacro

; Exceptions with error code pushed by CPU
%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    push dword %1
    jmp isr_common_stub
%endmacro

; IRQs
%macro IRQ_STUB 2   ; %1 = IRQ number (0-15), %2 = vector (32-47)
global irq%1
irq%1:
    cli
    push dword 0
    push dword %2 
    jmp irq_common_stub
%endmacro

; 32 stubs generation

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

; Generate 16 IRQ stubs on vectors 32-47

%assign i 0
%rep 16
    IRQ_STUB i, (32 + i)
%assign i i + 1
%endrep

ISR_NOERR 0x80 ; software test vector (128)

; Common stubs

isr_common_stub:
    pusha

    mov ax, ds
    push eax ; save data segment

    mov ax, 0x10 ; kernel data selector    ; COMPAT ONLY WITH THE 32-BIT GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp  
    call isr_handler
    add esp, 4

    pop eax ; restore data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8 
    sti
    iret

irq_common_stub:
    pusha

    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    add esp, 4

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    sti
    iret

section .bss
align 8
idt_table: resb 8 * 256

section .data
idt_descriptor:
    dw 8 * 256 - 1
    dd idt_table

section .text

%macro set_idt_gate 3   ; %1 = vector, %2 = handler label, %3 = type_attributes
    mov eax, %2
    mov word  [idt_table + (%1) * 8], ax
    mov word  [idt_table + (%1) * 8 + 2], 0x08 ; kernel code
    mov byte  [idt_table + (%1) * 8 + 4], 0
    mov byte  [idt_table + (%1) * 8 + 5], %3
    shr eax, 16
    mov word  [idt_table + (%1) * 8 + 6], ax
%endmacro

cpu_interrupt:
    int 0x80
    ret

idt_init:
    set_idt_gate 0,  isr0,  0x8E
    set_idt_gate 1,  isr1,  0x8E
    set_idt_gate 2,  isr2,  0x8E
    set_idt_gate 3,  isr3,  0x8E
    set_idt_gate 4,  isr4,  0x8E
    set_idt_gate 5,  isr5,  0x8E
    set_idt_gate 6,  isr6,  0x8E
    set_idt_gate 7,  isr7,  0x8E
    set_idt_gate 8,  isr8,  0x8E
    set_idt_gate 9,  isr9,  0x8E
    set_idt_gate 10, isr10, 0x8E
    set_idt_gate 11, isr11, 0x8E
    set_idt_gate 12, isr12, 0x8E
    set_idt_gate 13, isr13, 0x8E
    set_idt_gate 14, isr14, 0x8E
    set_idt_gate 15, isr15, 0x8E
    set_idt_gate 16, isr16, 0x8E
    set_idt_gate 17, isr17, 0x8E
    set_idt_gate 18, isr18, 0x8E
    set_idt_gate 19, isr19, 0x8E
    set_idt_gate 20, isr20, 0x8E
    set_idt_gate 21, isr21, 0x8E
    set_idt_gate 22, isr22, 0x8E
    set_idt_gate 23, isr23, 0x8E
    set_idt_gate 24, isr24, 0x8E
    set_idt_gate 25, isr25, 0x8E
    set_idt_gate 26, isr26, 0x8E
    set_idt_gate 27, isr27, 0x8E
    set_idt_gate 28, isr28, 0x8E
    set_idt_gate 29, isr29, 0x8E
    set_idt_gate 30, isr30, 0x8E
    set_idt_gate 31, isr31, 0x8E

    %assign i 0
    %rep 16
        set_idt_gate (32 + i), irq %+ i, 0x8E
    %assign i i + 1
    %endrep

    set_idt_gate 0x80, isr0x80, 0x8E

    lidt [idt_descriptor]
    ret

    ; FOR CRASHTESTING
    global clear_vector13_present
    
    clear_vector13_present:
        set_idt_gate 13, isr13, 0x0E
        ret
    ; ---