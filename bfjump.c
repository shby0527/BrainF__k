#include "bfjump.h"
#include "bfstack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const char *source_name(const char *source) {
  return source ? source : "<program>";
}

/* Report a bracket error using 1-based line and column numbers. */
static void report_unmatched(const char *code, size_t offset,
                             const char *source, char bracket,
                             size_t unclosed) {
  size_t line = 1;
  size_t column = 1;
  for (size_t i = 0; i < offset; i++) {
    if (code[i] == '\n') {
      line++;
      column = 1;
    } else {
      column++;
    }
  }
  if (bracket == '[') {
    fprintf(stderr, "%s:%zu:%zu: error: unmatched '[' (%zu unclosed)\n",
            source_name(source), line, column, unclosed);
  } else {
    fprintf(stderr, "%s:%zu:%zu: error: unmatched ']'\n", source_name(source),
            line, column);
  }
}

BFJumpTable *bf_jump_build(const char *code, size_t length,
                           const char *source) {
  if (!code) {
    return NULL;
  }

  BFJumpTable *table = (BFJumpTable *)malloc(sizeof(BFJumpTable));
  if (!table) {
    fprintf(stderr, "Error: Jump table allocation failed\n");
    return NULL;
  }
  table->length = length;
  table->jump = NULL;
  if (length > SIZE_MAX / sizeof(size_t)) {
    fprintf(stderr, "Error: Program too large\n");
    free(table);
    return NULL;
  }
  if (length > 0) {
    table->jump = (size_t *)malloc(length * sizeof(size_t));
    if (!table->jump) {
      fprintf(stderr, "Error: Jump table allocation failed\n");
      free(table);
      return NULL;
    }
    for (size_t i = 0; i < length; i++) {
      table->jump[i] = SIZE_MAX;
    }
  }

  BFStackContext *opens = bf_stack_create(64);
  if (!opens) {
    fprintf(stderr, "Error: Stack creation failed\n");
    bf_jump_destroy(table);
    return NULL;
  }

  for (size_t pc = 0; pc < length; pc++) {
    if (code[pc] == '[') {
      if (bf_stack_push(opens, pc) != 0) {
        fprintf(stderr, "%s: error: cannot check brackets\n",
                source_name(source));
        goto fail;
      }
    } else if (code[pc] == ']') {
      size_t open = 0;
      if (bf_stack_pop(opens, &open) != 0) {
        report_unmatched(code, pc, source, ']', 0);
        goto fail;
      }
      table->jump[open] = pc;
      table->jump[pc] = open;
    }
  }

  if (bf_stack_count(opens) > 0) {
    size_t open = 0;
    (void)bf_stack_peek(opens, &open);
    report_unmatched(code, open, source, '[', bf_stack_count(opens));
    goto fail;
  }

  bf_stack_destroy(opens);
  return table;

fail:
  bf_stack_destroy(opens);
  bf_jump_destroy(table);
  return NULL;
}

void bf_jump_destroy(BFJumpTable *table) {
  if (!table) {
    return;
  }
  free(table->jump);
  free(table);
}
