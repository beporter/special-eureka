// Ref: [hello_world.c](https://gist.github.com/beporter/0aeb1c8a404c4bcc9041)
// Ref: [getopt](https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/)
// Ref: [recursive dir size in c](https://stackoverflow.com/a/30379506/70876)

#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int dirSize(char* path, unsigned long long* sum) {
  char currentEntry[5000];
  struct stat currentFile;
  struct dirent *dirData;
  DIR *topDir;
  int status;
  int resp = EXIT_SUCCESS;

  topDir = opendir(path);

  if (topDir == NULL) {
    fprintf(stderr, "dirSize(): opendir failed. (%s)\n", path);
    return EXIT_FAILURE;
  }

  while ((dirData = readdir(topDir))) {
    // Skip bad data.
    if (dirData == NULL) {
      const unsigned int err = errno;
      fprintf(stderr, "dirSize(): readdir failed. (%u)\n", err);
      resp = EXIT_FAILURE;
      continue;
    }

    // Process directories.
    if (dirData->d_type == DT_DIR) {
      // Skip `.` and `..`.
      if (strncmp(dirData->d_name, ".", 3) == 0 || strncmp(dirData->d_name, "..", 3) == 0) {
        //fprintf(stderr, "dirSize(): Skipping .|.. dir.\n");
        continue;
      }

      strcpy(currentEntry, path);
      strcat(currentEntry, "/");
      strcat(currentEntry, dirData->d_name);

      fprintf(stderr, "dirSize(): Recursing into dir. (%s)\n", currentEntry);
      if (dirSize(currentEntry, sum) == EXIT_FAILURE) {
        resp = EXIT_FAILURE;
      }
    } else {
      strcpy(currentEntry, path);
      strcat(currentEntry, "/");
      strcat(currentEntry, dirData->d_name);

      status = stat(currentEntry, &currentFile);
      if (status < 0) {
        const unsigned int err = errno;
        fprintf(stderr, "dirSize(): stat failed: (%s) (%u)\n", currentEntry, err);
        resp = EXIT_FAILURE;
        continue;
      }

      (*sum) += currentFile.st_size;
      fprintf(stderr, "dirSize(): File size (%s): (%lli)\n", currentEntry, (long long)currentFile.st_size);
    }
  }

  closedir(topDir);
  return resp;
}

int main(int argc, char *argv[]) {

  printf("Path size scanner.\n");

  if (argc != 2) {
    fprintf(stderr, "\nProvide a filesystem path as first argument.");
    return 1;
  }

  char path[strlen(argv[1])];
  strcpy(path, argv[1]);
  printf("\nScanning %s", path);

  unsigned long long total = 0;
  if (dirSize(path, &total) == EXIT_SUCCESS) {
    printf("\nDone. Bytes: %lli", total);
    return EXIT_SUCCESS;
  }

  printf("\nFailed. Bytes: %lli", total);
  return EXIT_FAILURE;
}
