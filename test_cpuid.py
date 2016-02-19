""" Testing cpuid module
"""

import numpy as np

from cpuid import get_vendor, get_classifiers, supports_avx, get_cpuid

from numpy.testing import (assert_almost_equal,
                           assert_array_equal)

from nose.tools import (assert_true, assert_false, assert_raises,
                        assert_equal, assert_not_equal)


def test_smoke():
    vendor = get_vendor()
    classifiers = get_classifiers()
    avx = supports_avx()
    cpuid = get_cpuid(0)
