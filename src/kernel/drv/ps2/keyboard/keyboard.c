#include <arch/x86-64/kernel/cpu/idt/idt.h> // IDT table & interrupt handlers
#include <kernel/kernel_load.h> // kernel
#include <qemu/io.h> // QEMU console output
#include <stdint.h> // uint8_t, uint16_t, uint32_t
#include <etc/kblayout/kblayout.h> // keyboard layout wire-up (+ KeyCode enum) 
#include <drv/ps2/keyboard/keyboard.h> // PS/2 keyboard driver header
#include <cpu/interrupt/pic.h> // PIC remapping & IRQ masking/unmasking
#include <cpu/interrupt/isr.h> // interrupt service routines & handlers
#include <io/stdio.h> // no - this is not for printing (not coded in the driver); this is used only for port io
#include <stdbool.h> // boolean var type :)

// #define SC_MAX 0x58 // max sc for set1 (US_QWERTY)
#define SC_MAX (sizeof(sc_set1_to_keycode) / sizeof(sc_set1_to_keycode[0]) - 1) // max sc for set1 (C can math!  OwO)
// ^ TUNA uncomment this later when you implement the full scancode table & more layouts for now just use the hardcoded vvalue above

const char poop[] = "shit"; // totally needed, it improves the performance of the keyboard driver by 0 (a lot!)
// enpoopification < enshittification
int boot_improve = 2 + 2; // totally needed, it improves the boot time by 0.000000000000000000002 + 0.000000000000000000002 seconds!

static const KeyCode sc_set1_to_keycode[] = {
    [0x01] = KEY_ESC,
    [0x02] = KEY_1,
    [0x03] = KEY_2,
    [0x04] = KEY_3,
    [0x05] = KEY_4,
    [0x06] = KEY_5,
    [0x07] = KEY_6,
    [0x08] = KEY_7,
    [0x09] = KEY_8,
    [0x0A] = KEY_9,
    [0x0B] = KEY_0,
    [0x0C] = KEY_MINUS,
    [0x0D] = KEY_EQUAL,
    [0x0E] = KEY_BACKSPACE,
    [0x0F] = KEY_TAB,
    [0x10] = KEY_Q,
    [0x11] = KEY_W,
    [0x12] = KEY_E,
    [0x13] = KEY_R,
    [0x14] = KEY_T,
    [0x15] = KEY_Y,
    [0x16] = KEY_U,
    [0x17] = KEY_I,
    [0x18] = KEY_O,
    [0x19] = KEY_P,
    [0x1E] = KEY_A,
    [0x1F] = KEY_S,
    [0x20] = KEY_D,
    [0x21] = KEY_F,
    [0x22] = KEY_G,
    [0x23] = KEY_H,
    [0x24] = KEY_J,
    [0x25] = KEY_K,
    [0x26] = KEY_L,
    [0x27] = KEY_SEMICOLON,
    [0x28] = KEY_APOSTROPHE,
    [0x29] = KEY_GRAVE,
    [0x2A] = KEY_LSHIFT,
    [0x36] = KEY_RSHIFT,
    [0x2B] = KEY_BACKSLASH,
    [0x2C] = KEY_Z,
    [0x2D] = KEY_X,
    [0x2E] = KEY_C,
    [0x2F] = KEY_V,
    [0x30] = KEY_B,
    [0x31] = KEY_N,
    [0x32] = KEY_M,
    [0x33] = KEY_COMMA,
    [0x34] = KEY_DOT,
    [0x35] = KEY_SLASH,
    [0x3A] = KEY_CAPS,
    [0x1A] = KEY_LEFTBRACKET,
    [0x1B] = KEY_RIGHTBRACKET,
    [0x1C] = KEY_ENTER,
    [0x39] = KEY_SPACE,
    // ... (aDd MoRe KeYs As NeEdEd)
    // how many fucking lines i have to write this is ridiculous
    // i should just use a fucking array of strings or something lol -_-
    // or voice typing, no fucking keyboard drivers
    
};


// keyboard_callback is translating the raw PS/2 bytes to KeyCode enums and then calling the registered callback function with the KeyCode ^^
static void keyboard_callback(registers_t *regs) {
    qemu_console_write("[..] IRQ1 fired\n");
    uint8_t scancode = inb(0x60); // read the scancode from the PS/2 data port
    
    bool pressed = !(scancode & 0x80);
    uint8_t make_code = scancode & 0x7F;

    if (make_code > SC_MAX) return; // NAH fuck you, dont crash the kernel if the scancode is out of bounds dumbie dumbie
                                                    // at least it can panic and not just crash the kernel, but still, dont do that, its rude >:(
    KeyCode code = sc_set1_to_keycode[make_code];
    kblayout_handle_key_event(code, pressed);

    (void)regs; // better (void)warranty; oh yeah

}

void init_ps2_keyboard(void) {
    pic_clear_mask(1); // unmask IRQ1
    register_interrupt_handler(IRQ(1), keyboard_callback); // INTERRUPT THE CPU he is busy, he will be mad but I DONT CARE lol
}