#include <io/stdio.h>
#include <cpu/control.h>
#include <stdint.h>
#include <kernel/panic.h>

void panic(const char msg[], uint8_t ecode) {
    static const char hex[] = "0123456789ABCDEF";
    char error_code[] = {
        hex[(ecode >> 4) & 0x0F],
        hex[ecode & 0x0F],
        '\0'
    };
    
    clear();
    kprint("\n[ PANIC ] ");
    kprint(msg);
    kprint("\n");
    kprint("Error Code: 0x");
    kprint(error_code);
    kprint("\n");

    kprint("\nSystem halted.");
    fullstop();
}