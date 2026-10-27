#ifndef CPUID_H
#define CPUID_H
#include <stdint.h>

extern char cpu_vendor[49];
void init_cpu_vendor(); // Function to initialize the CPU vendor string

#endif