# -*- Mode: Python -*-
"""
Read CPU information using CPUID instruction.
"""

from collections import namedtuple

from libc.stdint cimport uint32_t

cdef extern from "cpuid.h":
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


cdef class X86Info:
    cdef:
        readonly e_registers_t reg0, reg1
        readonly char vendor[32]
        readonly int stepping, model, family, processor_type
        readonly int extended_model, extended_family
        readonly int model_display, family_display

    def __cinit__(self):
        cdef:
            cpu_classifiers_t cpu_classifiers
        read_cpuid(0, &self.reg0)
        read_cpuid(1, &self.reg1)
        read_vendor_string(self.reg0, self.vendor)
        read_classifiers(self.reg1, &cpu_classifiers)
        self.stepping = cpu_classifiers.stepping
        self.model = cpu_classifiers.model
        self.family = cpu_classifiers.family
        self.processor_type = cpu_classifiers.processor_type
        self.extended_model = cpu_classifiers.extended_model
        self.extended_family = cpu_classifiers.extended_family
        # Implement algorithm in
        # http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-2a-manual.html
        self.model_display = (self.model if self.family not in (6, 15) else
                              (self.extended_model << 4) + self.model)
        self.family_display = (self.family if self.family != 15 else
                               (self.extended_family << 4) + self.family)

    property supports_avx:
        def __get__(self):
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
            return bool(os_supports_avx(self.reg1))


info = X86Info()
