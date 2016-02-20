""" Testing cpuid module
"""

from cpuid import info

from cpuinfo.cpuinfo import get_cpu_info

CPU_INFO = get_cpu_info()



def test_against_cpuinfo():
    assert info.vendor == CPU_INFO['vendor_id']
    for attr_name in ('extended_family', 'extended_model', 'stepping',
                      'processor_type'):
        assert getattr(info, attr_name) == CPU_INFO[attr_name]
    # See:
    # http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-2a-manual.html
    assert CPU_INFO['model'] == info.model + (info.extended_model << 4)
    assert CPU_INFO['family'] == info.family + (info.extended_family << 4)


def test_smoke():
    avx = info.supports_avx
    cpuid_0 = info.reg0
    cpuid_1 = info.reg1
