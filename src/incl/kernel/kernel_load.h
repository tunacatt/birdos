// kernel_load.h
#ifndef KERNEL_LOAD_H
#define KERNEL_LOAD_H

extern void gdt_init(void);
extern void cpu_interrupt(void);

void kernel_load(void);


#endif