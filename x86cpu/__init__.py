""" x86cpu namespace
"""

from .cpuinfo import info, cpuid

from ._version import get_versions
__version__ = get_versions()['version']
del get_versions
