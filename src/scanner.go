package main

import (
	"fmt"
	"io/ioutil"
	"os"
)

func dirSize(path string) (uint64, error) {
	var sum uint64 = 0
	var subdirSum uint64 = 0
	var files []os.FileInfo
	var err error

	files, err = ioutil.ReadDir(path)

	for _, f := range files {
		if f.IsDir() && f.Name() != "." && f.Name() != ".." {
			subdirSum, err = dirSize(path + "/" + f.Name())
			sum += uint64(subdirSum)
		} else {
			sum += uint64(f.Size())
		}
	}

	return sum, err
}

func main() {
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s <path>\n", os.Args[0])
		os.Exit(1)
	}

	var path = os.Args[1]
	var err error
	var sum uint64

	fmt.Printf("Scanning dir (%s)\n", path)

	sum, err = dirSize(path)

	if err != nil {
		fmt.Println(err)
		fmt.Printf("Failed. %d\n", sum)
		os.Exit(1)
	}

	fmt.Printf("Done. %d\n", sum)
	os.Exit(0)
}
