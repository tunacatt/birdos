// kernel_load.c
// I dont see sharp, i just see
//        C   #    ,        C 
#include <kernel/kernel_load.h>
#include <io/stdio.h>
#include <qemu/io.h>
#include <cpu/control.h>
#include <cpu/cpuid.h>
#include <shell/vga_term/terminal.h>
#include <stdint.h>
#include <stddef.h>
#include <arch/x86-64/kernel/cpu/idt/idt.h>
#include <cpu/interrupt/pic.h>
#include <cpu/interrupt/isr.h>
#include <drv/ps2/keyboard/keyboard.h>
#include <cpu/check.h>

extern void gdt_init(void);
extern void cpu_interrupt(void);

void kernel_main(void); // You must compile kernel_load and kernel_main into one object

void kernel_load(void) {

    gdt_init();
    kprint("Loading BirdOS kernel...\n");
    pic_remap();
    idt_init();
    init_ps2_keyboard();
    pic_clear_mask(1);
    __asm__ volatile ("sti");

    clear();
    term_cursor_shape();

    kernel_main();
}
