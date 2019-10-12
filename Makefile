# Ref: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

build/scanner : builddir
	gcc -o $@ src/scanner.c -Isrc -Wall

builddir :
	test -d build || mkdir build

clean :
	rm -rf build/*
