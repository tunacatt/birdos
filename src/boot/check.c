#include <stdint.h>
#include <stdbool.h>

volatile uint16_t* vga = (volatile uint16_t*)0xB8000;

static void kprint(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        vga[i] = (uint16_t)str[i] | (uint16_t)(0x07 << 8);
        i++;
    }
}

static inline void cpuid(uint32_t leaf, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    __asm__ volatile (
        "cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "a" (leaf)
    );
}

void get_cpu_vendor(char* vendor) {
    uint32_t regs[12];
    uint32_t eax, ebx, ecx, edx;
    
    cpuid(0x80000002, &eax, &ebx, &ecx, &edx);
    regs[0] = eax; regs[1] = ebx; regs[2] = ecx; regs[3] = edx;

    cpuid(0x80000003, &eax, &ebx, &ecx, &edx);
    regs[4] = eax; regs[5] = ebx; regs[6] = ecx; regs[7] = edx;

    cpuid(0x80000004, &eax, &ebx, &ecx, &edx);
    regs[8] = eax; regs[9] = ebx; regs[10] = ecx; regs[11] = edx;

    char* p = (char*) regs;
    int i = 0, j = 0;

    while (p[i] == ' ') i++;
    while (i < 48 && p[i] != '\0') {
        vendor[j++] = p[i++];
    }
    vendor[j] = '\0';
}

char cpu_vendor[49];



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



void cpu_check(void) {
    get_cpu_vendor(cpu_vendor);

    if (!cpu_supports_long_mode() || !cpu_supports_pae()) {
        kprint("[ PANIC ] This computer doesn't support 64-bit long mode.");
        for (;;) __asm__ volatile ("cli\n\thlt");
    }
}