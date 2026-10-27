#include <cpu/interrupt/pic.h>
#include <io/stdio.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10   // begin init sqeuence

#define ICW4_8086 0x01   // 8086/88 mode

#define PIC_EOI 0x20

void pic_remap(void) {
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);   // master PIC: IRQ0-7  -> vectors 32-39
    io_wait();
    outb(PIC2_DATA, 0x28);   // slave PIC:  IRQ8-15 -> vectors 40-47
    io_wait();

    outb(PIC1_DATA, 0x04);   // tell master there's a slave on IRQ2
    io_wait();
    outb(PIC2_DATA, 0x02);   // tell slave its own cascade identity
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_set_mask(uint8_t irq) {
    uint16_t port = irq < 8 ? PIC1_DATA : PIC2_DATA;
    uint8_t bit = irq < 8 ? irq : (uint8_t)(irq - 8);
    uint8_t value = inb(port) | (uint8_t)(1 << bit);
    outb(port, value);
}

void pic_clear_mask(uint8_t irq) {
    uint16_t port = irq < 8 ? PIC1_DATA : PIC2_DATA;
    uint8_t bit = irq < 8 ? irq : (uint8_t)(irq - 8);
    uint8_t value = inb(port) & (uint8_t)~(1 << bit);
    outb(port, value);
}