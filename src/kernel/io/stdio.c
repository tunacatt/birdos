#include <shell/vga_term/terminal.h>
#include <io/stdio.h>

extern void clear(void);

void kprint(const char *str) {
    term_write(str);
}