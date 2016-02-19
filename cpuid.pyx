# -*- Mode: Python -*-
"""
Get cpuid
"""

from libc.stdint cimport uint32_t

cdef extern from "_cpuid.h":
    ctypedef struct cpuid_t:
        uint32_t eax
        uint32_t ebx
        uint32_t ecx
        uint32_t edx
    void read_cpuid(uint32_t eax, cpuid_t* res)
    void get_vendor_string(cpuid_t, char vendor[])


def get_cpuid(uint32_t op):
    cdef:
        cpuid_t reg
    read_cpuid(op, &reg)
    return reg


def get_vendor():
    cdef:
        cpuid_t reg
        char name[13]
    read_cpuid(0, &reg)
    get_vendor_string(reg, name)
    return name
