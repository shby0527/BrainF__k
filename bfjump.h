#ifndef BF_JUMP_H
#define BF_JUMP_H

#include <stddef.h>

/* Precompiled bracket jump table for a Brainfuck program.
 *
 * jump[i] holds the offset of the matching bracket when code[i] is '[' or
 * ']', and SIZE_MAX for every other byte. */
typedef struct BFJumpTable {
  size_t *jump;
  size_t length;
} BFJumpTable;

/* Validates that brackets are balanced and builds the jump table for
 * code[0..length). source is only used in diagnostics and may be NULL.
 * Returns NULL on error after printing a diagnostic to stderr. */
BFJumpTable *bf_jump_build(const char *code, size_t length, const char *source);

void bf_jump_destroy(BFJumpTable *table);

#endif
