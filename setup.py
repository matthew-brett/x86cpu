import platform

if platform.machine() not in ('i386', 'i686', 'x86_64', 'x86', 'AMD64', 'AMD32'):
    raise RuntimeError('x86cpu only builds on x86 CPUs')

from os.path import join as pjoin
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import versioneer
ext_modules = [Extension("x86cpu.cpuinfo",
                         [pjoin(*parts) for parts in (
                             ['x86cpu', 'cpuinfo.pyx'],
                             ['src', 'cpuid.c'],
                             ['src', 'os_restores_ymm.c'])],
                         include_dirs = ['src'])]

cmdclass=versioneer.get_cmdclass()
cmdclass['build_ext'] = build_ext

setup(
    version=versioneer.get_version(),
    cmdclass=cmdclass,
    name = 'x86cpu',
    ext_modules = ext_modules,
    packages     = ['x86cpu',
                    'x86cpu.tests']
)
