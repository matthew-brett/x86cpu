#ifndef __CPUID_H
#define __CPUID_H

#ifdef _MSC_VER
   typedef unsigned __int32 uint32_t
#else
   #include <stdint.h>
#endif

/*
 * struct reprensenting the cpuid flags as put in the register
 */
typedef struct {
        uint32_t eax;
        uint32_t ebx;
        uint32_t ecx;
        uint32_t edx;
} e_registers_t;

typedef struct {
    int stepping;
    int model;
    int family;
    int processor_type;
    int extended_model;
    int extended_family;
} cpu_classifiers_t;

void read_cpuid(uint32_t eax, e_registers_t* res);
void read_vendor_string(e_registers_t, char[]);
void read_classifiers(e_registers_t, cpu_classifiers_t*);
int os_restores_ymm(void);
int os_supports_avx(e_registers_t cpuid_1);

#endif
