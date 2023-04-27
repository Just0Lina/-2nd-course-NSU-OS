#include <unistd.h>
//  watch -d -n1 "./pagemap2 | grep -A10 heap"

#include <sys/wait.h>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int p_global = 101;
int main(int argc, char** argv) {
  int pid;
  int p_local = 100;
  printf("p_local: %p (%d), p_global: %p (%d)\n", &p_local, p_local, &p_global,
         p_global);
  printf("pid: %d\n", getpid());

  sleep(10);
  pid = fork();
  if (!pid) {
    printf("I'm a child (pid %d)\n", getpid());

    printf("I'm a child pid. Parent of pid=%d is pid=%d\n", getpid(),
           getppid());
    printf("p_local: %p (%d), p_global: %p (%d)\n", &p_local, p_local,
           &p_global, p_global);
    printf("Before change\n");
    sleep(10);

    p_global = 15;
    p_local = 1;
    printf("p_local: %p (%d), p_global: %p (%d)\n", &p_local, p_local,
           &p_global, p_global);
    sleep(10);

    exit(5);

  } else if (pid > 0) {
    printf("I'm a parent (pid %d)\n", getpid());
    printf("p_local: %p (%d), p_global: %p (%d)\n", &p_local, p_local,
           &p_global, p_global);
    sleep(25);
    printf("p_local: %p (%d), p_global: %p (%d)\n", &p_local, p_local,
           &p_global, p_global);
    int status;
    wait(&status);
    if (WIFEXITED(status)) {
      printf("Child exited with code %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      printf("Child terminated by signal %d\n", WTERMSIG(status));
    }
  } else {
    printf("Fork failed\n");
    return 1;
  }
  return EXIT_SUCCESS;
}