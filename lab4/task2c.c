#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int i = 0;

void f() {
  int a[4096];
  printf("%d: %d\n", i++, getpid());
  usleep(10000);
  f();
}

void signal_callback_handler(int signum) {
  printf("Caught signal %d\n", signum);

  exit(signum);
}

void f2() {
  long size = 0;
  const int block = 1024 * 1024;
  signal(SIGSEGV, signal_callback_handler);

  while (1) {
    malloc(block);
    size += block;
    printf("pid %d: size %ld\n", getpid(), size);
    usleep(500000);
    char* t =
        mmap(NULL, 10 * 4096, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // printf("%c\n", t[0]);
    // t[0] = 'v';
    munmap(t + 4 * 4096, 2 * 4096);
  }
}

void test_heap(int count) {
  // signal(SIGSEGV, signal_callback_handler);

  if (count == 1) {
    char* t =
        mmap(NULL, 10 * 4096, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    printf("%c\n", t[0]);
    // t[0] = 'v';
    // munmap(t + 4 * 4096, 2 * 4096);
    sleep(10);

    return;
  }

  int* arr = (int*)malloc(1024 * 1024);
  sleep(1);
  test_heap(--count);
  free(arr);
}

int main(int argc, char** argv) {
  printf("pid: %d\n", getpid());
  sleep(1);
  // f();
  test_heap(1);
  sleep(10);

  // f2();
  return EXIT_SUCCESS;
}
