""" Test helpers
"""

from subprocess import check_output

SYSCTL_KEY_TRANSLATIONS = dict(
    model='model_display',
    family='family_display',
    extmodel='extended_model',
    extfamily='extended_family',
    features='flags')


def get_sysctl_cpu():
    sysctl_text = check_output(['sysctl', '-a']).encode('utf8')
    info = {}
    for line in sysctl_text.splitlines():
        if not line.startswith('machdep.cpu.'):
            continue
        line = line.strip()[len('machdep.cpu.'):]
        key, value = line.split(': ', 1)
        key = SYSCTL_KEY_TRANSLATIONS.get(key, key)
        try:
            value = int(value)
        except ValueError:
            pass
        info[key] = value
    return info


PCPUINFO_KEY_TRANSLATIONS = dict(
    vendor_id='vendor',
    model='model_display',
    family='family_display')


def get_proc_cpuinfo():
    with open('/proc/cpuinfo', 'rt') as fobj:
        pci_lines = fobj.readlines()
    info = {}
    for line in pci_lines:
        line = line.strip()
        if line == '':  # End of first processor
            break
        key, value = line.split(':', 1)
        key, value = key.strip(), value.strip()
        key = PCPUINFO_KEY_TRANSLATIONS.get(key, key)
        try:
            value = int(value)
        except ValueError:
            pass
        info[key] = value
    return info


def get_wmic_cpu():
    pass
