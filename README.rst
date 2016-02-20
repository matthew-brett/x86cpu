##############################
Collect information on x86 CPU
##############################

Uses ``cpuid`` instruction to get information about CPU.

Queries OS as well as ``cpuid`` to see if the OS / CPU supports AVX
instructions.

**********
Quickstart
**********

>>> from x86cpu import info
>>> print(info.model_display, info.family_display)
(69, 6)
>>> print(info.vendor)
GenuineIntel

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
