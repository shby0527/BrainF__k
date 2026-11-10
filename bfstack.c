#include "bfstack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int bf_stack_reserve(BFStackContext *stack, size_t needed) {
  if (needed <= stack->capacity) {
    return 0;
  }
  size_t capacity = stack->capacity ? stack->capacity : 16;
  while (capacity < needed) {
    if (capacity > SIZE_MAX / 2) {
      capacity = needed;
      break;
    }
    capacity *= 2;
  }
  size_t *addr = (size_t *)realloc(stack->addr, capacity * sizeof(size_t));
  if (!addr) {
    fprintf(stderr, "Error: Stack allocation failed\n");
    return -1;
  }
  stack->addr = addr;
  stack->capacity = capacity;
  return 0;
}

BFStackContext *bf_stack_create(size_t initial_capacity) {
  BFStackContext *stack = (BFStackContext *)malloc(sizeof(BFStackContext));
  if (!stack) {
    return NULL;
  }
  stack->offset = 0;
  stack->capacity = 0;
  stack->addr = NULL;
  if (initial_capacity > 0 && bf_stack_reserve(stack, initial_capacity) != 0) {
    free(stack);
    return NULL;
  }
  return stack;
}

void bf_stack_destroy(BFStackContext *stack) {
  if (stack) {
    free(stack->addr);
    free(stack);
  }
}

int bf_stack_push(BFStackContext *stack, size_t addr) {
  if (!stack) {
    return -1;
  }
  if (bf_stack_reserve(stack, stack->offset + 1) != 0) {
    return -1;
  }
  stack->addr[stack->offset++] = addr;
  return 0;
}

int bf_stack_pop(BFStackContext *stack, size_t *value) {
  if (!stack || !value || stack->offset == 0) {
    return -1;
  }
  *value = stack->addr[--stack->offset];
  return 0;
}

int bf_stack_peek(const BFStackContext *stack, size_t *value) {
  if (!stack || !value || stack->offset == 0) {
    return -1;
  }
  *value = stack->addr[stack->offset - 1];
  return 0;
}

size_t bf_stack_count(const BFStackContext *stack) {
  return stack ? stack->offset : 0;
}
