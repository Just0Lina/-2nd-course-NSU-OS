#include <unistd.h>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int my_global_not_inited;
int my_global_inited = 10;
const int my_global_const = 20;

int* f2() {
  int b = 10;
  int* a = &b;
  return a;
}

void f() {
  int my_local;
  static int my_static_not_inited;
  static int my_static_inited = 5;
  const int my_local_const = 30;

  printf("\nfunction variables: \n");
  printf("local:              %p (%d)\n", &my_local, my_local);
  printf("static inited:      %p (%d)\n", &my_static_inited, my_static_inited);
  printf("static not inited:  %p (%d)\n", &my_static_not_inited,
         my_static_not_inited);
  printf("local const:       %p (%d)\n", &my_local_const, my_local_const);
}

void f3() {
  char* buffer = malloc(100);
  strcpy(buffer, "hello world");
  printf("buffer:           %s\n", buffer);

  free(buffer);
  printf("buffer:           %s\n", buffer);

  buffer = malloc(100);
  strcpy(buffer, "hello world");
  printf("buffer:           %s\n", buffer);

  buffer += sizeof(buffer) / 2;
  // free(buffer);
  printf("buffer:           %s\n", buffer);
}

void f4() {
  char* ptr = getenv("EXAMPLE");
  if (ptr != NULL)
    printf("EXAMPLE - %s\n", ptr);
  else
    return;
  ptr = "HELLO";
  printf("EXAMPLE %s\n", ptr);
  char* ptr2 = getenv("EXAMPLE");
  if (ptr != NULL) printf("EXAMPLE - %s\n", ptr);
}

int main(int argc, char** argv) {
  printf("function f():: %p\n", f);
  printf("\nglobal variables: \n");

  printf("global inited:              %p (%d)\n", &my_global_inited,
         my_global_inited);
  printf("global not inited:          %p (%d)\n", &my_global_not_inited,
         my_global_not_inited);
  printf("global const:               %p (%d)\n", &my_global_const,
         my_global_const);

  int* a = f2();
  printf("f2 returned:               %p (%d)\n", a, *a);
  f();
  f3();
  f4();

  printf("pid: %d\n", getpid());
  sleep(360);
  return EXIT_SUCCESS;
}