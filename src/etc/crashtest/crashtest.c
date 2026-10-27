#include <io/stdio.h>
#include <arch/x86-64/kernel/cpu/idt/idt.h>
#include <etc/crashtest/crashtest.h>

extern void clear_vector13_present(void);

void divide_by_zero(void) {
    __asm__ volatile (
        "xorl %%edx, %%edx\n\t"
        "movl $1, %%eax\n\t"
        "xorl %%ecx, %%ecx\n\t"
        "divl %%ecx"
        :
        :
        : "eax", "edx", "ecx", "cc"
    );

    kprint("Impossible");
}

void invalid_opcode(void) {
    __asm__ volatile ("ud2");
}

void breakpoint(void) {
    __asm__ volatile ("int3");
}

void double_fault(void) {
    clear_vector13_present();
    __asm__ volatile ("int $0x0D");
}