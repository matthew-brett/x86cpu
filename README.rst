##############################
Collect information on x86 CPU
##############################

Uses `cpuid <https://en.wikipedia.org/wiki/CPUID>`_ instruction to get
information about CPU.

Queries `OS as well as cpuid
<https://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Operating_system_support>`_
to see if the OS / CPU supports AVX instructions.

**********
Quickstart
**********

>>> from x86cpu import info
>>> print(info.model_display, info.family_display)
(69, 6)
>>> print(info.vendor)
GenuineIntel
>>> print(info.brand)
'Intel(R) Core(TM) i5-4250U CPU @ 1.30GHz'

You can run the ``cpuid`` command directly.  The argument to ``cpuid`` goes
into the ``EAX`` register before calling the CPUID instruction:

>>> from x86cpu import cpuid
>>> cpuid(1)
{'eax': 263761L, 'ebx': 17827840L, 'ecx': 2147154879L, 'edx': 3219913727L}

Some CPUID commands also care about the value in the ``ECX`` register.  You
can set this with a second optional argument to ``cpuid``:

>>> cpuid(13, 1)
{'eax': 1, 'ebx': 0, 'ecx': 0, 'edx': 0}

The package installs a command line tool ``x86report`` giving output like
this::

    $ x86report
    x86cpu report
    -------------
    brand            : Intel(R) Core(TM) i5-4250U CPU @ 1.30GHz
    vendor           : GenuineIntel
    model (display)  : 69
    family (display) : 6
    model            : 5
    family           : 6
    extended model   : 4
    extended family  : 0
    stepping         : 1
    processor type   : 0
    signature        : 263761
    MMX              : True
    3DNow!           : True
    SSE              : True
    SSE2             : True
    SSE3             : True
    SSSE3            : True
    SSE4.1           : True
    SSE4.2           : True
    supports AVX     : True
    supports AVX2    : True

****
Code
****

See https://github.com/matthew-brett/x86cpu

Released under the BSD two-clause license - see the file ``LICENSE`` in the
source distribution.

The latest released version is at https://pypi.python.org/pypi/x86cpu

*******
Support
*******

Please put up issues on the `x86cpu issue tracker
<https://github.com/matthew-brett/x86cpu/issues>`_.
