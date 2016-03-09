/*
 * Reporting information for reporting on CPU capabilities
 */

#include <stdio.h>
#include <stdlib.h>

#include "cpuinfo.h"

#define BINT(x) (x ? "True" : "False")

int main(int argc, void* argv)
{
    int ok;
    cpu_info_t i;

    ok = collect_info(&i);
    if (!ok)
    {
        printf("CPU does not have cpuid instruction\n");
        exit(-1);
    }
    printf("x86cpu report\n"
            "-------------\n"
            "brand            : %s\n"
            "vendor           : %s\n"
            "model (display)  : %d\n"
            "family (display) : %d\n"
            "model            : %d\n"
            "family           : %d\n"
            "extended model   : %d\n"
            "extended family  : %d\n"
            "stepping         : %d\n"
            "processor type   : %d\n"
            "signature        : %d\n"
            "MMX              : %s\n"
            "3DNow!           : %s\n"
            "SSE              : %s\n"
            "SSE2             : %s\n"
            "SSE3             : %s\n"
            "SSSE3            : %s\n"
            "SSE4.1           : %s\n"
            "SSE4.2           : %s\n"
            "supports AVX     : %s\n"
            "supports AVX2    : %s\n",
        i.brand, i.vendor,
        i.model_display, i.family_display,
        i.model, i.family,
        i.extended_model, i.extended_family,
        i.stepping, i.processor_type,
        i.signature,
        BINT(i.has_mmx), BINT(i.has_3dnow),
        BINT(i.has_sse), BINT(i.has_sse2),
        BINT(i.has_sse3), BINT(i.has_ssse3),
        BINT(i.has_sse4_1), BINT(i.has_sse4_2),
        BINT(i.supports_avx),
        BINT(i.supports_avx2)
            );
    exit(0);
}
