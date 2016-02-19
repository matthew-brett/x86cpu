/*
 * TODO:
 *  - test for cpuid availability
 *  - test for OS support (tricky)
 */

#include <stdint.h>
#include <string.h>

#include "_cpuid.h"


void read_cpuid(uint32_t op, cpuid_t* reg)
{
#if defined(_MSC_VER)
    int cpu_info[4] = {-1};
    __cpuid(cpu_info, (int)op);
    reg->eax = cpu_info[0];
    reg->ebx = cpu_info[1];
    reg->ecx = cpu_info[2];
    reg->edx = cpu_info[3];
#elif defined(__i386__) && defined(__PIC__)
    /* see:
     * https://software.intel.com/en-us/articles/how-to-detect-new-instruction-support-in-the-4th-generation-intel-core-processor-family
     */
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

char* get_vendor_string(cpuid_t cpuid)
{
    /* Fetch vendor string from ebx, edx, ecx */
    char vendor[13];
    uint32_t* char_as_int=(uint32_t*)vendor;

    *(char_as_int++) = cpuid.ebx;
    *(char_as_int++) = cpuid.edx;
    *(char_as_int++) = cpuid.ecx;
    vendor[12] = '\0';
    return vendor;
}
