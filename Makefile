all : test

test : ext
	py.test .

ext :
	python setup.py build_ext -i

clean :
	rm -rf x86cpu\*.o x86cpu\*.so x86cpu\cpuinfo.c
