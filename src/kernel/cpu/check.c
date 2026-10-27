#include <stdint.h>
#include <stdbool.h>
#include <io/stdio.h>
#include <cpu/control.h>
#include <cpu/check.h>

static inline bool cpu_supports_long_mode(void) {
    uint32_t eax, edx;

    __asm__ volatile ("cpuid" : "=a"(eax) : "a"(0x80000000) : "ebx", "ecx", "edx");
    if (eax < 0x80000001) return 0;

    __asm__ volatile ("cpuid" : "=a"(eax), "=d"(edx) : "a"(0x80000001) : "ebx", "ecx");
    return (edx & (1 << 29)) != 0;
}

static inline bool cpu_supports_pae(void) {
    uint32_t eax, edx;
    __asm__ volatile ("cpuid" : "=a"(eax), "=d"(edx) : "a"(1) : "ebx", "ecx");
    return (edx & (1 << 6)) != 0;
}

bool cpu_supported(void) {
    if (!cpu_supports_long_mode() || !cpu_supports_pae()) return false;
    return true;
}

void cpu_unsupported(void) {
    clear();
    kprint("\n");
    kprint("[ ERR ] This CPU doesn't support x86-64/long mode. BirdOS requires it.");
    /*
    kprint("System Halted.");
    fullstop();
    */
}