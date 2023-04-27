#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEMORY_SIZE 1024

typedef struct mem_block {
  size_t size;
  struct mem_block *next;
  int free;
} mem_block_t;

static void *memory = NULL;
static mem_block_t *head = NULL;

void init() {
  memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  head = (mem_block_t *)memory;
  head->size = MEMORY_SIZE - sizeof(mem_block_t);
  head->next = NULL;
  head->free = 1;
}

void *my_malloc(size_t size) {
  mem_block_t *current;
  void *result = NULL;
  if (head == NULL) {
    init();
  }

  current = head;
  while (current) {
    if (current->free && current->size >= size) {
      if (current->size > size + sizeof(mem_block_t)) {
        mem_block_t *new_block =
            (mem_block_t *)((char *)current + sizeof(mem_block_t) + size);
        new_block->size = current->size - size - sizeof(mem_block_t);
        new_block->next = current->next;
        new_block->free = 1;
        current->next = new_block;
        current->size = size;
      }
      current->free = 0;
      result = (void *)((char *)current + sizeof(mem_block_t));
      break;
    }
    current = current->next;
  }
  if (result != NULL) {
    printf("Allocated %zu bytes\n", size);
  } else {
    printf("Allocation failed\n");
  }
  return result;
}

void my_free(void *ptr) {
  mem_block_t *current;

  if (ptr == NULL) {
    return;
  }

  current = (mem_block_t *)((char *)ptr - sizeof(mem_block_t));
  if (current->free) {
    return;
  }
  current->free = 1;
  printf("Freed memory");

  // Merge free blocks
  current = head;
  while (current) {
    if (current->free && current->next && current->next->free) {
      current->size += sizeof(mem_block_t) + current->next->size;
      current->next = current->next->next;
    }
    current = current->next;
  }
}

int main() {
  int *p1 = my_malloc(100);
  memset(p1, 0x00, 100);
  printf("Malloc: p1 %p\n", p1);
  for (int i = 0; i < 25; ++i) {
    p1[i] = i;
    printf("%d ", p1[i]);
  }
  printf("\n");

  void *p2 = my_malloc(200);
  printf("Malloc: p2 %p\n", p2);

  void *p3 = my_malloc(300);
  printf("Malloc: p3 %p\n", p3);

  void *p4 = my_malloc(400);
  printf("Malloc: p4 %p\n", p4);

  my_free(p3);

  void *p5 = my_malloc(250);
  printf("Malloc: p5 %p\n", p5);

  my_free(p1);
  my_free(p3);
  my_free(p4);
  my_free(p5);

  int *p6 = my_malloc(80);
  for (int i = 0; i < 20; ++i) {
    printf("%d ", p6[i]);
  }
  printf("\n");
  printf("Malloc: p6 %p\n", p6);
  for (int i = 0; i < 20; ++i) {
    p6[i] = 20 - i;
    printf("%d ", p6[i]);
  }
  printf("\n");

  my_free(p6);

  return 0;
}