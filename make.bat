del x86cpu\*.pyd
del x86cpu\cpuinfo.c
python setup.py build_ext -i
py.test .
