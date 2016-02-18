# -*- Mode: Python -*-
"""
Get cpuid
"""
from __future__ import print_function

from libc.stdint cimport uint32_t

cdef extern from "_cpuid.h":
    ctypedef struct cpuid_t:
        uint32_t eax
        uint32_t ebx
        uint32_t ecx
        uint32_t edx
    void read_cpuid(uint32_t eax, cpuid_t res)
    int get_vendor_string(cpuid_t, char vendor[])


def get_cpuid(uint32_t op):
    cdef:
        cpuid_t reg
    read_cpuid(op, reg)
    return reg
