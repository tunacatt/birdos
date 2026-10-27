// idt64.c - Interrupt descriptor table for 64-bit Long mode
#include <stdint.h>

#define IDT_VECTOR_COUNT 256

struct idt_entry64 {
    uint16_t offset_1; // low offset bits 
    uint16_t selector; // segment selector in GDT
    uint8_t ist; // Interrupt Stack Table
    uint8_t type_attr; // type attribute
    uint16_t offset_2; // mid offset
    uint32_t offset_3; // high offset
    uint32_t zero; // reserved
} idt[256];

extern void *isr_stub_table[256];

void set_idt_gate(uint8_t vector, uintptr_t handler) {
    idt[vector].offset_1 = handler & 0xFFFF;
    idt[vector].selector = 0x08;
    idt[vector].ist = 0;
    idt[vector].type_attr = 0x8E;
    idt[vector].offset_2 = (handler >> 16) & 0xFFFF;
    idt[vector].offset_3 = (handler >> 32) & 0xFFFFFFFF;
    idt[vector].zero = 0; // since when 0 equals 0 ???
}

void idt_init(void) {
    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, (uintptr_t)isr_stub_table[i]);
    }
}