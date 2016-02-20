all : test

test : ext
	py.test .

ext :
	pip install -e .

clean :
	rm -rf x86cpu\*.o x86cpu\*.so x86cpu\cpuinfo.c
