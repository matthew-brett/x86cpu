#ifndef __CPUID_H
#define __CPUID_H

#ifdef _MSC_VER
/* MSVC < 2010 does not have stdint.h */
typedef unsigned __int32 uint32_t;
#else
#include <stdint.h>
#endif

/*
 * struct to hold values copied from the extended 32-bit registers
 */
typedef struct {
        uint32_t eax;
        uint32_t ebx;
        uint32_t ecx;
        uint32_t edx;
} e_registers_t;

/*
 * struct for CPU classification values
 */
typedef struct {
    int stepping;
    int model;
    int family;
    int processor_type;
    int extended_model;
    int extended_family;
} cpu_classifiers_t;

void read_cpuid(uint32_t, uint32_t, e_registers_t*);
void read_brand_string(char []);
void read_vendor_string(e_registers_t, char[]);
void read_classifiers(e_registers_t, cpu_classifiers_t*);
int os_restores_ymm(void);
int os_supports_avx(e_registers_t cpuid_1);

/*
 * Set bit a through bit b (inclusive), as long as 0 <= a <= 31 and 0 <= b <= 31.
 * From
 * http://stackoverflow.com/questions/8774567/c-macro-to-create-a-bit-mask-possible-and-have-i-found-a-gcc-bug
 */
#define BIT_MASK(a, b) (((unsigned) -1 >> (31 - (b))) & ~((1U << (a)) - 1))

#endif
