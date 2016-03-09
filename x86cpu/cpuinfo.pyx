# -*- Mode: Python -*-
"""
Read CPU information using cpuid instruction.
"""

from __future__ import print_function


cdef extern from "cpuid.h":
    ctypedef unsigned int uint32_t
    ctypedef struct e_registers_t:
        uint32_t eax
        uint32_t ebx
        uint32_t ecx
        uint32_t edx
    ctypedef struct cpu_classifiers_t:
        int stepping
        int model
        int family
        int processor_type
        int extended_model
        int extended_family
    int c_has_cpuid "has_cpuid" ()
    void read_cpuid(uint32_t eax, uint32_t ecx, e_registers_t* res)
    uint32_t BIT_MASK(uint32_t, uint32_t)
    uint32_t HAS_BIT(uint32_t, uint32_t)


cdef extern from "cpuinfo.h":
    ctypedef struct  cpu_info_t:
        e_registers_t reg0, reg1, reg7
        char *vendor, *brand
        int stepping, model, family, processor_type
        int extended_model, extended_family
        int model_display, family_display, signature
        int max_basic_arg
        bint has_mmx, has_sse, has_sse2, has_sse3, has_3dnow, has_ssse3
        bint has_sse4_1, has_sse4_2
        bint has_reg7  # True if cpuid supports cpuid(7)
        bint supports_avx, supports_avx2

    bint collect_info(cpu_info_t *)


cpdef int _has_bit(uint32_t val, int bit):
    # Instantiate macro in order to test in Python
    return HAS_BIT(val, bit)


def _bit_mask(a, b):
    # Instantiate macro in order to test in Python
    return BIT_MASK(a, b)



class X86Info(object):
    report_template = """\
x86cpu report
-------------
brand            : {i.brand}
vendor           : {i.vendor}
model (display)  : {i.model_display}
family (display) : {i.family_display}
model            : {i.model}
family           : {i.family}
extended model   : {i.extended_model}
extended family  : {i.extended_family}
stepping         : {i.stepping}
processor type   : {i.processor_type}
signature        : {i.signature}
MMX              : {i.has_mmx}
3DNow!           : {i.has_3dnow}
SSE              : {i.has_sse}
SSE2             : {i.has_sse2}
SSE3             : {i.has_sse3}
SSSE3            : {i.has_ssse3}
SSE4.1           : {i.has_sse4_1}
SSE4.2           : {i.has_sse4_2}
supports AVX     : {i.supports_avx}
supports AVX2    : {i.supports_avx2}
"""

    def __init__(self):
        cdef:
            cpu_info_t info
        if not collect_info(&info):
            raise RuntimeError("cpuid not present on this CPU")
        self.__dict__.update(info)
        # Convert byte strings to strings
        for key in ('vendor', 'brand'):
            self.__dict__[key] = self.__dict__[key].decode('latin1')

    def report(self):
        return self.report_template.format(i=self)


info = X86Info() if has_cpuid() else None


def print_report():
    print("No cpuid on this CPU" if info is None else info.report())


cpdef bint has_cpuid():
    """ Return True if this CPU has the cpuid instruction
    """
    return c_has_cpuid()


cpdef e_registers_t cpuid(uint32_t op, uint32_t sub_op=0):
    """ Register values from cpuid instruction with EAX=`op`, ECX=`sub_op`
    """
    cdef e_registers_t registers
    if not has_cpuid():
        raise RuntimeError('CPU does not have cpuid instruction')
    read_cpuid(op, sub_op, &registers)
    return registers
