#include "bflink.h"
#include "bfstack.h"
#include "config.h"
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "bf version %d.%d Usage: %s <filename>\n", BF_VERSION_MAJOR, BF_VERSION_MINOR, argv[0]);
    return EXIT_FAILURE;
  }

  const char *filename = argv[1];
  int fd = open(filename, O_RDONLY);

  BFMemoryContext *context = bf_memory_create();
  if (!context) {
    fprintf(stderr, "Error: Memory context creation failed\n");
    close(fd);
    return EXIT_FAILURE;
  }
  BFStackContext *stack = bf_stack_create(1024);
  if (!stack) {
    fprintf(stderr, "Error: Stack creation failed\n");
    bf_memory_destroy(context);
    close(fd);
    return EXIT_FAILURE;
  }
  struct stat st;
  stat(filename, &st);
  void *buffer = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (buffer == MAP_FAILED) {
    fprintf(stderr, "Error: Memory mapping failed\n");
    bf_memory_destroy(context);
    bf_stack_destroy(stack);
    close(fd);
    return EXIT_FAILURE;
  }
  const char *code = (const char *)buffer;
  size_t pc = 0;
  while (pc < st.st_size) {
    char command = code[pc];
    switch (command) {
    case '>':
      move_next(context);
      break;
    case '<':
      move_previous(context);
      break;
    case '+':
      set_current_data(context, get_current_data(context) + 1);
      break;
    case '-':
      set_current_data(context, get_current_data(context) - 1);
      break;
    case '.':
      putchar(get_current_data(context));
      break;
    case ',':
      set_current_data(context, getchar());
      break;
    case '[':
      if (get_current_data(context) == 0) {
        // jump to the matching ']'
        unsigned int matched = 0;
        while (pc < st.st_size) {
          if (code[pc] == '[') {
            matched++;
          } else if (code[pc] == ']') {
            matched--;
            if (matched == 0) {
              break;
            }
          }
          pc++;
        }
      } else {
        push(stack, pc);
      }
      break;
    case ']':
      if (get_current_data(context) != 0) {
        pc = peek(stack);
      } else {
        pop(stack);
      }
      break;
    default:
      break;
    }
    pc++;
  }
  munmap(buffer, st.st_size);
  close(fd);
  bf_memory_destroy(context);
  bf_stack_destroy(stack);
  return EXIT_SUCCESS;
}