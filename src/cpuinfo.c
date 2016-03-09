/*
 * Collating information for reporting on CPU capabilities
 */

#include "cpuinfo.h"

int collect_info(cpu_info_t *info)
{
    cpu_classifiers_t cpu_classifiers;
    uint32_t edx1, ecx1;

    if (! has_cpuid() )
    {
        return 0;
    }
    read_cpuid(0, 0, &(info->reg0));
    info->max_basic_arg = info->reg0.eax;
    read_cpuid(1, 0, &(info->reg1));
    info->has_reg7 = info->max_basic_arg >= 7;
    if (info->has_reg7)
    {
        read_cpuid(7, 0, &(info->reg7));
    }
    read_vendor_string(info->reg0, info->vendor);
    read_brand_string(info->brand);
    read_classifiers(info->reg1, &cpu_classifiers);
    info->stepping = cpu_classifiers.stepping;
    info->model = cpu_classifiers.model;
    info->family = cpu_classifiers.family;
    info->processor_type = cpu_classifiers.processor_type;
    info->extended_model = cpu_classifiers.extended_model;
    info->extended_family = cpu_classifiers.extended_family;
    /* Implement algorithm in
     http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-2a-manual.html
     */
    if ((info->family == 6) | (info->family == 15))
    {
        info->model_display = (info->extended_model << 4) + info->model;
    } else {
        info->model_display = info->model;
    }
    if (info->family == 15)
    {
        info->family_display = (info->extended_family << 4) + info->family;
    } else {
        info->family_display = info->family;
    }
    info->signature = info->reg1.eax;
    edx1 = info->reg1.edx;
    ecx1 = info->reg1.ecx;
    info->has_mmx = HAS_BIT(edx1, 23);
    info->has_sse = HAS_BIT(edx1, 25);
    info->has_sse2 = HAS_BIT(edx1, 26);
    info->has_3dnow = HAS_BIT(edx1, 26);
    info->has_sse3 = HAS_BIT(ecx1, 0);
    info->has_ssse3 = HAS_BIT(ecx1, 9);
    info->has_sse4_1 = HAS_BIT(ecx1, 19);
    info->has_sse4_2 = HAS_BIT(ecx1, 20);
    /* supports_avx
     * True if CPU and OS support AVX instructions
     *
     * The cpuid(1) ECX register tells us if the CPU supports AVX.
     *
     * For the OS to support AVX, it needs to preserve the AVX YMM registers
     * when doing a context switch.  In order to do this, the needs to support
     * the relevant instructions, and the OS needs to know to preserve these
     * registers.

     * See:

     * https://en.wikipedia.org/wiki/cpuid
     * https://en.wikipedia.org/wiki/Advanced_Vector_Extensions
     * https://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled/
     */
    info->supports_avx = os_supports_avx(info->reg1);
    info->supports_avx2 = 0;
    info->supports_avx2 = (
            info->supports_avx &
            info->has_reg7 &
            HAS_BIT(info->reg7.ebx, 5));
    return 1;
}
