# -*- Mode: Python -*-
"""
Get cpuid
"""

from collections import namedtuple

from libc.stdint cimport uint32_t

cdef extern from "_cpuid.h":
    ctypedef struct cpuid_t:
        uint32_t eax
        uint32_t ebx
        uint32_t ecx
        uint32_t edx
    void read_cpuid(uint32_t eax, cpuid_t* res)
    void get_vendor_string(cpuid_t, char vendor[])
    int os_restores_ymm()


cpdef cpuid_t get_cpuid(uint32_t op):
    cdef:
        cpuid_t reg
    read_cpuid(op, &reg)
    return reg


def get_vendor():
    """ Return vendor string by querying cpuid
    """
    cdef char name[13]
    get_vendor_string(get_cpuid(0), name)
    return name


def _all_set(val, bits):
    for bit in bits:
        if not ((1 << bit) & val):
            return False
    return True


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
    return bool(_supports_avx(get_cpuid(1)))


cdef int _supports_avx(cpuid_t reg1):
    if not _all_set(reg1.ecx, [26, 27, 28]):
        return False
    return os_restores_ymm()


cdef int _bitmask(uint32_t a, uint32_t b, uint32_t c):
    return (a >> b) & c


cpu_ids = namedtuple('cpu_ids',
                     ['family', 'model', 'ext_family', 'ext_model',
                      'stepping', 'processor_type'])


def get_classifiers():
    """ Return CPU family, model, extended model / family, stepping, type
    """
    eax = get_cpuid(1).eax
    ext_family = _bitmask(eax, 20, 0xff)
    ext_model  = _bitmask(eax, 16, 0x0f)
    ptype = _bitmask(eax, 12, 0x03)
    family = _bitmask(eax, 8, 0x0f)
    model = _bitmask(eax,  4, 0x0f)
    stepping  = _bitmask(eax,  0, 0x0f)
    return cpu_ids(family=family, model=model, ext_family=ext_family,
                   ext_model=ext_model, stepping=stepping,
                   processor_type=ptype)
