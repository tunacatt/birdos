#ifndef PANIC_H
#define PANIC_H

#include <stdint.h>

void panic(const char *msg, uint8_t ecode);

#endif