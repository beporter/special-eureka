# Research

A place to start organizing my thoughts around this project.


## Known Limits

These influence how to store data like file/total size.

* [Operating system maximum disk size limits](https://www.hardwaresecrets.com/hard-disk-drives-capacity-limits/6/)
* `stat.st_size` [libc](https://pubs.opengroup.org/onlinepubs/009695399/functions/stat.html), [darwin](https://github.com/apple/darwin-xnu/blob/master/bsd/sys/stat.h), [linux](https://linux.die.net/man/2/stat), [FreeBSD](https://www.freebsd.org/cgi/man.cgi?query=stat&sektion=2)

## Prior Art

Apps that already have to scan files/directories recursively.

* [qDirStat](https://github.com/shundhammer/qdirstat)
* [`du`](https://github.com/coreutils/coreutils/blob/master/src/du.c)
* [Duc](https://github.com/zevv/duc)
* [nftw](https://stackoverflow.com/a/472727/70876)
* [readdir example](https://stackoverflow.com/a/30379506/70876)


## Design Ideas

* Can't store the entire structure in RAM when there might be millions of directories.
  * Need to cache to... file? db?
  * A tree makes sense, to mirror the directory structure being scanned.
  * That cache needs to be easy to read/update at each subtree.

* The list of dirs to scan could be a queue.
  * Subdirs to scan get added to the queue.
  * A client can request a priority entry (front of queue).
  * Separate thread/process for "worker" that does the job of getting file sizes and populating the cache.

* API interface
  * Callers need to be able to request a scan from a specific root path.
  * Callers should be able to request _results_ from a scan before its complete
    * Need to have a payload format for partial results. "send what we have so far."

* Try building something crude around `ls` as a start? Doesn't need to be fast or memory efficient yet.

* Data to store:
  * dir basename
  * full path? inode?
  * pointer to parent
  * cumulative size (bytes) might not be calculated yet.
  * list of immediate children?
