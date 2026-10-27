#include <cpu/cpuid.h>
#include <stdint.h>

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


// GLOBAL variable for the CPU vendor string
char cpu_vendor[49];
// YOU NEED TO CALL THIS FUNCTION SOMEWHERE (either sysinfo or kernel_load) TO INITIALIZE THE CPU VENDOR STRING
void init_cpu_vendor() {
    get_cpu_vendor(cpu_vendor);
}