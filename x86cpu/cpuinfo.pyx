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
    void read_brand_string(char [])
    void read_vendor_string(e_registers_t, char[])
    void read_classifiers(e_registers_t, cpu_classifiers_t*)
    int os_supports_avx(e_registers_t cpuid_1)
    uint32_t BIT_MASK(uint32_t, uint32_t)


def _bit_mask(a, b):
    # Instantiate macro in order to test in Python
    return BIT_MASK(a, b)


cdef int _has_bit(uint32_t val, int bit):
    return (val & 1 << bit) != 0


cdef class X86Info:
    cdef:
        readonly e_registers_t reg0, reg1, reg7
        readonly object vendor, brand
        readonly int stepping, model, family, processor_type
        readonly int extended_model, extended_family
        readonly int model_display, family_display, signature
        readonly int max_basic_arg
        readonly bint has_mmx, has_sse, has_sse2, has_sse3, has_3dnow, has_ssse3
        readonly bint has_sse4_1, has_sse4_2
        bint has_reg7  # True if cpuid supports cpuid(7)

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

    def __cinit__(self):
        cdef:
            cpu_classifiers_t cpu_classifiers
            char _vendor[32], _brand[64]
        read_cpuid(0, 0, &self.reg0)
        self.max_basic_arg = self.reg0.eax
        read_cpuid(1, 0, &self.reg1)
        self.has_reg7 = self.max_basic_arg >= 7
        if self.has_reg7:
            read_cpuid(7, 0, &self.reg7)
        read_vendor_string(self.reg0, _vendor)
        read_brand_string(_brand)
        self.brand = _brand.decode('latin1')
        self.vendor = _vendor.decode('latin1')
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
        self.signature = self.reg1.eax
        edx1, ecx1 = self.reg1.edx, self.reg1.ecx
        self.has_mmx = _has_bit(edx1, 23)
        self.has_sse = _has_bit(edx1, 25)
        self.has_sse2 = _has_bit(edx1, 26)
        self.has_3dnow = _has_bit(edx1, 26)
        self.has_sse3 = _has_bit(ecx1, 0)
        self.has_ssse3 = _has_bit(ecx1, 9)
        self.has_sse4_1 = _has_bit(ecx1, 19)
        self.has_sse4_2 = _has_bit(ecx1, 20)

    property supports_avx:
        def __get__(self):
            """ Return True if CPU and OS support AVX instructions

            The cpuid(1) ECX register tells us if the CPU supports AVX.

            For the OS to support AVX, it needs to preserve the AVX YMM registers when
            doing a context switch.  In order to do this, the needs to support the
            relevant instructions, and the OS needs to know to preserve these
            registers.

            See:
            * https://en.wikipedia.org/wiki/cpuid
            * https://en.wikipedia.org/wiki/Advanced_Vector_Extensions
            * https://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled/

            Returns
            -------
            avx_ok : bool
                True if CPU and OS support AVX
            """
            return bool(os_supports_avx(self.reg1))

    property supports_avx2:
        """ True if we have AVX support and cpuid reports AVX2 on CPU
        """
        def __get__(self):
            return (self.supports_avx and
                    self.has_reg7 and
                    bool(_has_bit(self.reg7.ebx, 5)))

    def report(self):
        """ Return string giving report on CPU features
        """
        return self.report_template.format(i=self)


info = None if not has_cpuid() else X86Info()


def print_report():
    print("No cpuid on this CPU" if not has_cpuid() else info.report())


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
