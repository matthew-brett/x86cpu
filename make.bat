del *.pyd *.o cpuid.c
python setup.py build_ext -i
nosetests .
