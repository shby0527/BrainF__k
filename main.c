#include "bflink.h"
#include "bfjump.h"
#include "config.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "bf version %d.%d Usage: %s <filename>\n",
            BF_VERSION_MAJOR, BF_VERSION_MINOR, argv[0]);
    return EXIT_FAILURE;
  }

  const char *filename = argv[1];
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    perror(filename);
    return EXIT_FAILURE;
  }

  struct stat st;
  if (fstat(fd, &st) != 0) {
    perror(filename);
    close(fd);
    return EXIT_FAILURE;
  }
  if (st.st_size <= 0) {
    // An empty program is a valid no-op.
    close(fd);
    return EXIT_SUCCESS;
  }

  const size_t code_size = (size_t)st.st_size;
  void *buffer = mmap(NULL, code_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (buffer == MAP_FAILED) {
    perror("mmap");
    close(fd);
    return EXIT_FAILURE;
  }
  const char *code = (const char *)buffer;

  // Validate brackets and resolve all '[' / ']' pairs up front.
  BFJumpTable *jumps = bf_jump_build(code, code_size, filename);
  if (!jumps) {
    munmap(buffer, code_size);
    close(fd);
    return EXIT_FAILURE;
  }

  BFMemoryContext *context = bf_memory_create();
  if (!context) {
    fprintf(stderr, "Error: Memory context creation failed\n");
    bf_jump_destroy(jumps);
    munmap(buffer, code_size);
    close(fd);
    return EXIT_FAILURE;
  }

  size_t pc = 0;
  while (pc < code_size) {
    switch (code[pc]) {
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
      set_current_data(context, (unsigned char)getchar());
      break;
    case '[':
      if (get_current_data(context) == 0) {
        // Jump to the matching ']'; pc++ below moves past it.
        pc = jumps->jump[pc];
      }
      break;
    case ']':
      if (get_current_data(context) != 0) {
        // Jump back to the matching '['; pc++ below re-enters the body.
        pc = jumps->jump[pc];
      }
      break;
    default:
      break;
    }
    pc++;
  }

  bf_jump_destroy(jumps);
  munmap(buffer, code_size);
  close(fd);
  bf_memory_destroy(context);
  return EXIT_SUCCESS;
}
