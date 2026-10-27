#include <etc/sysinfo/sysinfo.h>
#include <stdint.h>
#include <kernel/kernel_load.h>
#include <io/stdio.h>
#include <cpu/cpuid.h>

#define MSR_IA32_EFER 0xC0000080u
#define EFER_LMA (1u << 10)

static void print_address(uintptr_t address) {
    static const char hex_digits[] = "0123456789abcdef";
    char text[2 + sizeof(address) * 2 + 1];
    const unsigned int digit_count = sizeof(address) * 2;

    text[0] = '0';
    text[1] = 'x';
    for (unsigned int i = 0; i < digit_count; i++) {
        unsigned int shift = (digit_count - 1 - i) * 4;
        text[2 + i] = hex_digits[(address >> shift) & 0xf];
    }
    text[2 + digit_count] = '\0';
    kprint(text);
}

static inline uint64_t rdmsr(uint32_t msr) {
    uint32_t lo, hi;

    __asm__ volatile (
        "rdmsr"
        : "=a"(lo), "=d"(hi)
        : "c"(msr)
    );

    return ((uint64_t)hi << 32) | lo;
}

int is_long_mode_active(void) {
    return (rdmsr(MSR_IA32_EFER) & EFER_LMA) != 0;
}

void print_sysinfo(void) {
    kprint("--- System Information ---\n");
    init_cpu_vendor();
    kprint("BirdOS Kernel version: 0.1.0 [built on " __DATE__ " " __TIME__ " UTC+8]\n");
    kprint("Kernel memory address: "); print_address((uintptr_t)&kernel_load); kprint("\n");
    kprint("CPU Execution mode: "); if (is_long_mode_active()) kprint("64-bit long mode\n"); else kprint("32-bit protected mode\n");
    kprint("CPUID: "); kprint(cpu_vendor); kprint("\n"); // CPUID
    // [...]

    kprint("--------------------------\n");
}