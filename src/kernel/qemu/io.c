#include <qemu/io.h>

void qemu_console_write(const char *str) {
     while (*str) {
         __asm__ volatile (
             "outb %%al, $0xE9\n\t"
             :
             : "a"(*str)
         );
         str++;
     }
 }