#include <arch/x86-64/kernel/cpu/idt/idt.h>
#include <stdint.h>
#include <cpu/control.h>

void hlt(void) {
    __asm__ volatile ("hlt");
}

void loophlt(void) {
    for (;;) hlt();
}

void fullstop(void) {
    __asm__ volatile ("cli\n\thlt");
}