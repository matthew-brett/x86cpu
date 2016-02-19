/*
 * TODO:
 *  - test for cpuid availability
 *  - test for OS support (tricky)
 */

#include <stdint.h>
#include <string.h>

#include "_cpuid.h"


void read_cpuid(uint32_t op, cpuid_t* reg){
#if defined(_MSC_VER)
    int cpu_info[4] = {-1};
    __cpuid(cpu_info, (int)op);
    reg->eax = cpu_info[0];
    reg->ebx = cpu_info[1];
    reg->ecx = cpu_info[2];
    reg->edx = cpu_info[3];
#elif defined(__i386__) && defined(__PIC__)
    __asm__ __volatile__
        ("mov %%ebx, %%edi;"
         "cpuid;"
         "xchgl %%ebx, %%edi;"
         : "=a" (reg->eax), "=D" (reg->ebx), "=c" (reg->ecx), "=d" (reg->edx) : "a" (op) : "cc");
#else
    __asm__ __volatile__
        ("cpuid": "=a" (reg->eax), "=b" (reg->ebx), "=c" (reg->ecx), "=d" (reg->edx) : "a" (op) : "cc");
#endif
}

void xgetbv(uint32_t op, uint32_t* eax_var, uint32_t* edx_var){
  /* Use binary code for xgetbv */
#if defined(_MSC_VER)
    __asm {
        mov ecx, [op]
        __asm _emit 0x0f __asm _emit 0x01 __asm _emit 0xd0
        mov [*eax_var], eax
        mov [*edx_var], edx
    }
#else
    __asm__ __volatile__
        (".byte 0x0f, 0x01, 0xd0": "=a" (*eax_var), "=d" (*edx_var) : "c" (op) : "cc");
#endif
}

void get_vendor_string(cpuid_t cpuid, char *vendor)
{
    /* Fetch vendor string from ebx, edx, ecx */
    memcpy(vendor, &(cpuid.ebx), 4);
    memcpy(vendor + 4, &(cpuid.edx), 4);
    memcpy(vendor + 8, &(cpuid.ecx), 4);
    vendor[12] = '\0';
}
