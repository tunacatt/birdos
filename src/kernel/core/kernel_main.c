// kernel_main.c
#include <kernel/kernel_load.h>
#include <cpu/interrupt/isr.h>
#include <arch/x86-64/kernel/cpu/idt/idt.h>
#include <cpu/interrupt/pic.h>
#include <drv/ps2/keyboard/keyboard.h>
#include <etc/sysinfo/sysinfo.h>
#include <io/stdio.h>
#include <qemu/io.h>
#include <cpu/control.h>
#include <shell/kshell/shell.h>
#include <stdint.h>

extern void gdt_init(void);
extern void cpu_interrupt(void);
/*
INTERRUPTING THE CPU:
- make sure gdt and idt are initialized, and the PIC is remapped and unmasked for the interrupt you want to test
- make a void like this: void handler(registers_t *regs) { ... } (e.g. print something etc.)
- then call register_interrupt_handler(0x80, handler); 
- then call cpu_interrupt(); to trigger the interrupt
- then you can check the QEMU console output to see if the interrupt fired successfully, or if the handler does something that
  requires an interrupt (e.g. input device), then check if it works (ONLY if the drivers are there, working as well)
*/
extern void hlt(void);
extern void loophlt(void);

void test_handler(registers_t *regs) {
    kprint("[ OK ] Interrupt fired!\n");
    qemu_console_write("[ OK ] Interrupt fired!\n");
    (void)regs; // F*CK YOU unused parameter warning
}

void welcome_msg(void) {
    kprint("Welcome to BirdOS!\n");
    kprint("For more information, see the BirdOS documentation and wiki, or the GitHub repo at https://github.com/tunacatt/birdos-next.\n");
}

void kernel_main(void) {
    kernel_load();
    kprint("[ OK ] SUCCESS: BirdOS Kernel initialized.\n");
    qemu_console_write("[ OK ] SUCCESS: BirdOS Kernel initialized.\n");
    kprint("No filesystem mounted, please scan for disks then mount a filesystem to manage files.\n");
    // qemu_console_write("[..] Before interrupt\n");
    // We for sure don't need the test interrupt, as there is input
    // Uncomment if you really want that, but that doesn't matter anymore in my opinion

    // register_interrupt_handler(0x80, test_handler);
    // cpu_interrupt();
    // qemu_console_write("[..] After interrupt\n");
    print_sysinfo();
    welcome_msg();
    shell_init(); // One of the last things you wanna do
    loophlt();
    
}