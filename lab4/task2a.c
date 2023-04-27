#include <unistd.h>
//  watch -d -n1 cat /proc/140754/maps
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char** argv) {
  printf("pid: %d\n", getpid());
  sleep(1);
  execv("./a.out", argv);
  printf("Hello, world!");
  return EXIT_SUCCESS;
}