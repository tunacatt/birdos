#ifndef PIC_H
#define PIC_H

#include <stdint.h>

// Remaps the 8259 PIC(s) so hardware IRQs 0-15 fire on vectors 32-47
// instead of the power-on default 0-15, which collides with CPU
// exceptions. Call this once, before idt_init()/sti - see the ordering
// note in kernel wiring instructions.
void pic_remap(void);

// Must be called at the end of every IRQ handler, or the PIC will
// never deliver another interrupt on that line (or any lower-priority
// line) again. 'irq' is the raw IRQ number (0-15), not the vector.
void pic_send_eoi(uint8_t irq);

// Masking/unmasking: a driver should unmask its own IRQ line once it's
// actually ready to handle interrupts, not before - that's what keeps
// keyboard.c "owning" IRQ1 rather than something else enabling it on
// the driver's behalf.
void pic_set_mask(uint8_t irq);
void pic_clear_mask(uint8_t irq);

#endif