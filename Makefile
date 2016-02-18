all : test

test : ext
	nosetests .

ext :
	python setup.py build_ext -i

clean :
	rm -rf *.o *.so
