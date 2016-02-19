#ifndef __CPUID_H
#define __CPUID_H

#include <stdint.h>

/*
 * struct reprensenting the cpuid flags as put in the register
 */
typedef struct {
        uint32_t eax;
        uint32_t ebx;
        uint32_t ecx;
        uint32_t edx;
} cpuid_t;

void read_cpuid(uint32_t eax, cpuid_t* res);
void get_vendor_string(cpuid_t, char []);
int os_restores_ymm(void);

#endif
