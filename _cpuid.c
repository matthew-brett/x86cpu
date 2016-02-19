/*
 * TODO:
 *  - test for cpuid availability
 *  - test for OS support (tricky)
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "_cpuid.h"

/*
 * Union to read bytes in 32 (intel) bits registers
 */
union _le_reg {
        uint8_t ccnt[4];
        uint32_t reg;
} __attribute__ ((packed));
typedef union _le_reg le_reg_t ;


inline void read_cpuid(uint32_t op, cpuid_t* reg){
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

/*
 * vendor should have at least CPUID_VENDOR_STRING_LEN characters
 */
void get_vendor_string(cpuid_t cpuid, char vendor[])
{
    int i;
    le_reg_t treg;

    treg.reg = cpuid.ebx;
    for (i = 0; i < 4; ++i) {
            vendor[i] = treg.ccnt[i];
    }

    treg.reg = cpuid.edx;
    for (i = 0; i < 4; ++i) {
            vendor[i+4] = treg.ccnt[i];
    }

    treg.reg = cpuid.ecx;
    for (i = 0; i < 4; ++i) {
            vendor[i+8] = treg.ccnt[i];
    }
    vendor[12] = '\0';
}
