#ifndef IDT_H
#define IDT_H

#define IDT_ENTRIES 256

#define IRQ_BASE 32
#define IRQ(n) (IRQ_BASE + (n))

#define IDT_RESERVED_COUNT 32

void idt_init(void);

#endif