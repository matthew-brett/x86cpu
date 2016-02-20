# -*- Mode: Python -*-
"""
Get cpuid
"""

from collections import namedtuple

from libc.stdint cimport uint32_t

cdef extern from "_cpuid.h":
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
    void read_cpuid(uint32_t eax, e_registers_t* res)
    void read_vendor_string(e_registers_t, char[])
    void read_classifiers(e_registers_t, cpu_classifiers_t*)
    int os_supports_avx(e_registers_t cpuid_1)


cpdef e_registers_t get_cpuid(uint32_t op):
    """ Return contents of extended registers after _cpuid call with EAX=`op`
    """
    cdef:
        e_registers_t reg
    read_cpuid(op, &reg)
    return reg


def get_vendor():
    """ Return vendor string by querying cpuid
    """
    cdef:
        char vendor[12]
    read_vendor_string(get_cpuid(0), vendor)
    return vendor


def supports_avx():
    """ Return True if CPU and OS support AVX instructions

    The cpuid(1) ECX register tells us if the CPU supports AVX.

    For the OS to support AVX, it needs to preserve the AVX YMM registers when
    doing a context switch.  In order to do this, the needs to support the
    relevant instructions, and the OS needs to know to preserve these
    registers.

    See:
    * https://en.wikipedia.org/wiki/CPUID
    * https://en.wikipedia.org/wiki/Advanced_Vector_Extensions
    * https://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled/

    Returns
    -------
    avx_ok : bool
        True if CPU and OS support AVX
    """
    return bool(os_supports_avx(get_cpuid(1)))


def get_classifiers():
    """ Return CPU family, model, extended model / family, stepping, type
    """
    cdef:
        cpu_classifiers_t cpu_classifiers
    read_classifiers(get_cpuid(1), &cpu_classifiers)
    return cpu_classifiers
