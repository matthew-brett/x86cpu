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

>>> from x86cpu import info, cpuid
>>> print(info.model_display, info.family_display)
(69, 6)
>>> print(info.vendor)
GenuineIntel
>>> print(info.brand)
'Intel(R) Core(TM) i5-4250U CPU @ 1.30GHz'

You can also run the ``cpuid`` command directly:

>>> cpuid(1)
{'eax': 263761L, 'ebx': 17827840L, 'ecx': 2147154879L, 'edx': 3219913727L}

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
