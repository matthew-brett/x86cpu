import platform

if platform.machine() not in ( 'i386', 'i686', 'x86_64', 'x86',
                              'AMD64', 'AMD32'):
    raise RuntimeError('x86cpu only builds on x86 CPUs')

from os.path import join as pjoin
import sys

# For some commands, always use setuptools.
if len(set(('develop', 'bdist_egg', 'bdist_rpm', 'bdist', 'bdist_dumb',
            'install_egg_info', 'egg_info', 'easy_install', 'bdist_wheel',
            'bdist_mpkg')).intersection(sys.argv)) > 0:
    import setuptools

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
)
