#include <kernel/kernel_load.h>
#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_COLOR  0x07 // Light grey on black

static uint16_t *const vga_buffer = (uint16_t*)0xB8000;
static uint8_t term_row = 0;
static uint8_t term_col = 0;

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

void term_cursor_shape(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0);

    outb(0x3D4, 0x0B);
    outb(0x3D5, 15);
}

static void term_update_cursor(void) {
    uint16_t pos = term_row * VGA_WIDTH + term_col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);

    outb(0x3D4, 0x0E);
    outb(0x3D5, pos >> 8);
}

static void term_scroll(void) {
    for (uint8_t y = 1; y < VGA_HEIGHT; y++)
        for (uint8_t x = 0; x < VGA_WIDTH; x++)
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];

    for (uint8_t x = 0; x < VGA_WIDTH; x++)
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', VGA_COLOR);

    term_row = VGA_HEIGHT - 1;
}

static void term_putchar(char c) {
    if (c == '\n') {
        term_col = 0;
        term_row++;
    } else if (c == '\b') {
        if (term_col > 0) {
            term_col--;
            vga_buffer[term_row * VGA_WIDTH + term_col] = vga_entry(' ', VGA_COLOR);
        }

    } else { 
        vga_buffer[term_row * VGA_WIDTH + term_col] = vga_entry(c, VGA_COLOR);
        term_col++;
        if (term_col >= VGA_WIDTH) {
            term_col = 0;
            term_row++;
        }
    }

    if (term_row >= VGA_HEIGHT)
        term_scroll();
    
    term_update_cursor();
}

void term_write(const char *str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        term_putchar(str[i]);
}

void clear(void) {
    for (uint8_t y = 0; y < VGA_HEIGHT; y++)
        for (uint8_t x = 0; x < VGA_WIDTH; x++)
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', VGA_COLOR);

    term_row = 0;
    term_col = 0;
}