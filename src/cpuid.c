/*
 * TODO:
 *  - test for cpuid availability
 *  - test for OS support (tricky)
 */

#include <string.h>

#include "cpuid.h"

/* shift a by b bits to the right, then mask with c */
#define SHIFT_MASK(a, b, c) ((((a) >> (b)) & (c)))

void read_cpuid(uint32_t op, e_registers_t* reg)
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

void read_vendor_string(e_registers_t cpuid_1, char* vendor)
{
    /* Read vendor string from ebx, edx, ecx
     * Registers in `cpuid_1` resulted from call to read_cpuid(1, &cpuid_1)
     */
    uint32_t* char_as_int=(uint32_t*)vendor;
    *(char_as_int++) = cpuid_1.ebx;
    *(char_as_int++) = cpuid_1.edx;
    *(char_as_int) = cpuid_1.ecx;
    vendor[12] = '\0';
}

void read_brand_string(char* brand)
{
    /*
     * https://en.wikipedia.org/wiki/CPUID#EAX.3D80000002h.2C80000003h.2C80000004h:_Processor_Brand_String
     */
    uint32_t* char_as_int=(uint32_t*)brand;
    int op;
    e_registers_t registers;
    read_cpuid(0x80000000, &registers);
    if (registers.eax < 0x80000004)
    {
        brand[0] = '\0';
    }
    for (op = 0x80000002; op < 0x80000005; op++)
    {
        read_cpuid(op, &registers);
        *(char_as_int++) = registers.eax;
        *(char_as_int++) = registers.ebx;
        *(char_as_int++) = registers.ecx;
        *(char_as_int++) = registers.edx;
    }
}

void read_classifiers(e_registers_t cpuid_1, cpu_classifiers_t* cpu_params)
{
    /*
    * 3:0 – Stepping
    * 7:4 – Model
    * 11:8 – Family
    * 13:12 – Processor Type
    * 19:16 – Extended Model
    * 27:20 – Extended Family
    * See:
    * https://en.wikipedia.org/wiki/CPUID#EAX.3D1:_Processor_Info_and_Feature_Bits
    * Page 3-191 of
    * http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-2a-manual.html
    */
    uint32_t eax = cpuid_1.eax;
    cpu_params->stepping = SHIFT_MASK(eax, 0, 0x0f);
    cpu_params->model = SHIFT_MASK(eax,  4, 0x0f);
    cpu_params->family = SHIFT_MASK(eax, 8, 0x0f);
    cpu_params->processor_type = SHIFT_MASK(eax, 12, 0x03);
    cpu_params->extended_model = SHIFT_MASK(eax, 16, 0x0f);
    cpu_params->extended_family = SHIFT_MASK(eax, 20, 0xff);
}

int os_supports_avx(e_registers_t cpuid_1)
{
    /*
     * The cpuid(1) ECX register tells us if the CPU supports AVX.
     *
     * For the OS to support AVX, it needs to preserve the AVX YMM registers
     * when doing a context switch.  In order to do this, the needs to support
     * the relevant instructions, and the OS needs to know to preserve these
     * registers.
     *
     * See:
     * https://en.wikipedia.org/wiki/CPUID
     * https://en.wikipedia.org/wiki/Advanced_Vector_Extensions
     * https://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled/
     */
    uint32_t mask = BIT_MASK(26, 28);
    return ((cpuid_1.ecx & mask) == mask) ? os_restores_ymm() : 0;
}
