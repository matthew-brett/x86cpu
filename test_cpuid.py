""" Testing cpuid module
"""

from cpuid import get_vendor, get_classifiers, supports_avx, get_cpuid

from cpuinfo.cpuinfo import get_cpu_info

CPU_INFO = get_cpu_info()

def test_smoke():
    avx = supports_avx()
    cpuid = get_cpuid(0)


def test_against_cpuinfo():
    assert get_vendor() == CPU_INFO['vendor_id']
    classifiers = get_classifiers()
    for attr_name in ('model', 'family', 'extended_family', 'extended_model',
                      'stepping', 'processor_type'):
        assert getattr(classifiers, attr_name) == CPU_INFO[attr_name]
