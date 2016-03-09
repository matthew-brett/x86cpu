/*
 * Header for reporting collected CPU information
 */
#ifndef __CPUINFO_H
#define __CPUINFO_H

#include "cpuid.h"

/*
 * struct for CPU information fields
 */
typedef struct {
    e_registers_t reg0, reg1, reg7;
    char vendor[32], brand[64];
    int stepping, model, family, processor_type;
    int extended_model, extended_family;
    int model_display, family_display, signature;
    int max_basic_arg;
    int has_mmx, has_sse, has_sse2, has_sse3, has_3dnow, has_ssse3;
    int has_sse4_1, has_sse4_2;
    int has_reg7;  /* True if cpuid supports cpuid(7) */
    int supports_avx, supports_avx2;
} cpu_info_t;

int collect_info(cpu_info_t *info);

#endif
