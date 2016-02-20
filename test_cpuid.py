""" Testing cpuid module
"""

from sys import platform as PLATFORM

from cpuid import info

import pytest

from helpers import get_sysctl_cpu, get_proc_cpuinfo, get_wmic_cpu

pytestmark = pytest.mark.skipif(
    PLATFORM not in ('darwin', 'win32') and
    not PLATFORM.startswith('linux'),
    reason='Valid platforms are OSX, Windows, Linux')

REF_INFO = {}

def setup_module():
    global REF_INFO
    if PLATFORM == 'darwin':
        REF_INFO.update(get_sysctl_cpu())
    elif PLATFORM.startswith('linux'):
        REF_INFO.update(get_proc_cpuinfo())
    elif PLATFORM == 'win32':
        REF_INFO.update(get_wmic_cpu())
    else:
        raise RuntimeError('Was not intending to test platform ' +
                           PLATFORM)


def test_against_ref():
    assert info.vendor == REF_INFO['vendor'].encode('latin1')
    for attr_name in ('extended_family', 'extended_model', 'stepping',
                      'model_display', 'family_display'):
        if attr_name in REF_INFO:
            assert getattr(info, attr_name) == REF_INFO[attr_name]


def test_smoke():
    avx = info.supports_avx
    cpuid_0 = info.reg0
    cpuid_1 = info.reg1
