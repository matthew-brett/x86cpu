all : test

test : ext
	py.test .

ext :
	python setup.py build_ext -i

clean :
	rm -rf *.o *.so
