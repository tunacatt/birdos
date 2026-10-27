
#ifndef CPU_INTERRUPT_ISR_H
#define CPU_INTERRUPT_ISR_H

#include <stdint.h>

#define IDT_ENTRIES 256

#define IRQ_BASE  32
#define IRQ_COUNT 16

typedef struct registers {
    uint64_t r15, r14, r13, r12;
    uint64_t r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rdx;
    uint64_t rcx, rbx, rax;

    uint64_t int_no;
    uint64_t err_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
} registers_t;

typedef void (*isr_handler_t)(registers_t *regs);

int register_interrupt_handler(uint8_t vector,
                               isr_handler_t handler);

void isr_handler(registers_t *regs);
void irq_handler(registers_t *regs);

#endif
