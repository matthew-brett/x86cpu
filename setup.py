from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules = [Extension("cpuid",
                         ["cpuid.pyx", "_cpuid.c", "_os_restores_ymm.c"])]

setup(
  name = 'cpuid',
  cmdclass = {'build_ext': build_ext},
  ext_modules = ext_modules
)
