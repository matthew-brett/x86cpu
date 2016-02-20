import platform

if platform.machine() not in ( 'i386', 'i686', 'x86_64', 'x86',
                              'AMD64', 'AMD32'):
    raise RuntimeError('x86cpu only builds on x86 CPUs')

import os
from os.path import join as pjoin, exists

# BEFORE importing distutils, remove MANIFEST. distutils doesn't properly
# update it when the contents of directories change.
if exists('MANIFEST'): os.remove('MANIFEST')

import setuptools
from distutils.core import setup
from distutils.extension import Extension

import versioneer
cmdclass=versioneer.get_cmdclass()

# Utilities to build from c or pyx files
from cythexts import cyproc_exts, get_pyx_sdist, stamped_pyx_ok
# Check this version is tested in .travis.yml
CYTHON_MIN_VERSION="0.18"

ext_modules = [Extension("x86cpu.cpuinfo",
                         [pjoin(*parts) for parts in (
                             ['x86cpu', 'cpuinfo.pyx'],
                             ['src', 'cpuid.c'],
                             ['src', 'os_restores_ymm.c'])],
                         include_dirs = ['src'])]

# Cython is a dependency for building extensions, iff we don't have stamped up
# pyx and c files.
build_ext, need_cython = cyproc_exts(ext_modules,
                                     CYTHON_MIN_VERSION,
                                     'pyx-stamps')
print('Cython?', need_cython)
print('Hashes', stamped_pyx_ok(ext_modules, 'pyx-stamps'))
cmdclass['build_ext'] = build_ext
cmdclass['sdist'] = get_pyx_sdist(cmdclass['sdist'])

setup(
    version=versioneer.get_version(),
    cmdclass=cmdclass,
    description='Collect information about x86 CPU',
    name = 'x86cpu',
    ext_modules = ext_modules,
    packages     = ['x86cpu',
                    'x86cpu.tests'],
    author='Matthew Brett',
    author_email='matthew.brett@gmail.com',
    maintainer='Matthew Brett',
    maintainer_email='matthew.brett@gmail.com',
    url='http://github.com/matthew-brett/x86cpu',
    license='BSD license',
    classifiers = [
        'Development Status :: 2 - Pre-Alpha',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX',
        'Operating System :: Unix',
        'Operating System :: MacOS',
    ],
    long_description = open('README.rst', 'rt').read(),
#   setup_requires=['cython'] if need_cython else [],
)
