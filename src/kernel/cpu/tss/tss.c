// TSS (This Shitty Shit)
#include <stdint.h>
#include <stddef.h>
#include <cpu/tss/tss.h>

typedef struct __attribute__((packed)) {
    uint32_t prev_tss;   // 0x00
    uint32_t esp0;       // 0x04: r0 stack ptr
    uint32_t ss0;        // 0x08 : r0 stack segment
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} tss_entry_t;

static tss_entry_t tss;

void tss_init(uint32_t kernel_stack) {
    for (size_t i = 0; i < sizeof(tss); ++i) {
        ((uint8_t *)&tss)[i] = 0;
    }
    tss.ss0 = 0x10;        // r0 data selector
    tss.esp0 = kernel_stack;

    // place offset beyond end of tss disable io permission bitmap
    tss.iomap_base = sizeof(tss);
}