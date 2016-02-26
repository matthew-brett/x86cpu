""" Testing cpuid module
"""

from sys import platform as PLATFORM, path
from os.path import dirname
path.append(dirname(__file__))

from x86cpu import info, cpuid
from x86cpu.cpuinfo import _bit_mask

import pytest

from info_getters import get_sysctl_cpu, get_proc_cpuinfo, get_wmic_cpu

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
    assert info.vendor == REF_INFO['vendor']
    for attr_name in ('extended_family', 'extended_model', 'stepping',
                      'model_display', 'family_display', 'signature',
                      'supports_avx', 'supports_avx2'):
        if attr_name in REF_INFO:
            assert getattr(info, attr_name) == REF_INFO[attr_name]
    for feature in ('sse', 'sse2', 'sse3', 'mmx', '3dnow', 'sse4_1', 'sse4_2'):
        if feature in REF_INFO['unknown_flags']:
            continue
        has_feature = feature in REF_INFO['flags']
        assert (getattr(info, 'has_' + feature) == has_feature)


def set_bits(bits):
    val = 0
    for bit in bits:
        val += 2**bit
    return val


def test_bitmask():
    for bit1 in range(32):
        for bit2 in range(bit1 + 1, 32):
            # Test bit1 through bit2 (inclusive)
            bits = range(bit1, bit2 + 1)
            assert _bit_mask(bit1, bit2) == set_bits(bits)


def test_smoke():
    avx = info.supports_avx

    def cmp_reg(a, b):
        # ebx appears to be incompletely defined for cpuid(1) call
        for regname in ('eax', 'ecx', 'edx'):
            assert a[regname] == b[regname]

    assert info.reg0 == cpuid(0)
    cmp_reg(info.reg1, cpuid(1))
    assert info.reg7 == cpuid(7)
    assert info.report() is not None
