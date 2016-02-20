""" Testing cpuid module
"""

from cpuid import info

from cpuinfo.cpuinfo import get_cpu_info

CPU_INFO = get_cpu_info()



def test_against_cpuinfo():
    assert info.vendor == CPU_INFO['vendor_id']
    for attr_name in ('model', 'family', 'extended_family', 'extended_model',
                      'stepping', 'processor_type'):
        assert getattr(info, attr_name) == CPU_INFO[attr_name]


def test_smoke():
    avx = info.supports_avx
    cpuid_0 = info.reg0
    cpuid_1 = info.reg1
