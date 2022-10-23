# Ref: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

EXECUTABLES = gcc go entr rustc
K := $(foreach exec,$(EXECUTABLES),\
  $(if $(shell which $(exec)),some string,$(error "No $(exec) in PATH")))

all : build/scanner build/scannerc build/scannerrs

build/scanner : builddir
	go build -o $@ src/scanner.go

build/scannerc : builddir
	gcc -o $@ src/scanner.c -Isrc -Wall

build/scannerrs : builddir
	rustc -o $@ src/scanner.rs

builddir :
	test -d build || mkdir build

clean :
	rm -rf build/*

watch :
	ls src/*.go | entr -c make

watchc :
	ls src/*.c | entr -c make
