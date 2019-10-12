# Ref: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

EXECUTABLES = gcc entr
K := $(foreach exec,$(EXECUTABLES),\
  $(if $(shell which $(exec)),some string,$(error "No $(exec) in PATH")))

build/scanner : builddir
	gcc -o $@ src/scanner.c -Isrc -Wall

builddir :
	test -d build || mkdir build

clean :
	rm -rf build/*

watch:
	ls src/*.c | entr -c make
