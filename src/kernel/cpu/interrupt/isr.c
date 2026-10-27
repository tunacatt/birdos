#include <cpu/interrupt/isr.h>
#include <arch/x86-64/kernel/cpu/idt/idt.h>
#include <cpu/interrupt/pic.h>
#include <kernel/kernel_load.h>
#include <io/stdio.h>
#include <kernel/panic.h>
#include <stdint.h>

#define IRQ_BASE 32
#define IRQ_COUNT 16

static isr_handler_t interrupt_handlers[IDT_ENTRIES];

// Index = vector number (0-31). Purely descriptive - used only to print
// something readable when an exception has no registered handler.
static const char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved"
};

// TUNA! please update this struct if changing the push order in stub gen
struct interrupt_frame {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rdx, rcx, rbx, rax;

    uint64_t vector;
    uint64_t errorcode;
    
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
};

int register_interrupt_handler(uint8_t n, isr_handler_t handler) {
    if (handler == 0) {
        // Additional protection
        return -1;
    }
    interrupt_handlers[n] = handler;
    return 0;
}

void isr_handler(registers_t *regs)
{
    uint64_t vector = regs->int_no;

    if (vector >= IDT_ENTRIES) {
        panic("Invalid interrupt vector", (int)vector);
        return;
    }

    if (interrupt_handlers[vector] != 0) {
        interrupt_handlers[vector](regs);
        return;
    }

    if (vector < 32) {
        panic(exception_messages[vector], (int)vector);
        return;
    }

    panic("Unhandled interrupt", (int)vector);
}

void irq_handler(registers_t *regs)
{
    uint64_t vector = regs->int_no;

    if (vector < IRQ_BASE ||
        vector >= IRQ_BASE + IRQ_COUNT) {
        panic("Invalid IRQ vector", (int)vector);
        return;
    }

    uint8_t irq = (uint8_t)(vector - IRQ_BASE);

    if (interrupt_handlers[vector] != 0) {
        interrupt_handlers[vector](regs);
    }

    pic_send_eoi(irq);
}