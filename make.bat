del x86cpu\*.pyd
del x86cpu\cpuinfo.c
pip install -e .
py.test .
