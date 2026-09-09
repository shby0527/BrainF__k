#include "bfstack.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

BFStackContext *bf_stack_create(size_t size) {
  BFStackContext *stack =
      (BFStackContext *)malloc(sizeof(BFStackContext) + size * sizeof(size_t));
  if (!stack) {
    return NULL;
  }
  stack->offset = 0;
  stack->size = size;
  memset(stack->addr, 0, size * sizeof(size_t));
  return stack;
}

void bf_stack_destroy(BFStackContext *stack) {
  if (stack) {
    free(stack);
  }
}

void push(BFStackContext *stack, size_t addr) {
  if (!stack) {
    fprintf(stderr, "Error: Stack is NULL\n");
    return;
  }
  if (stack->offset < stack->size) {
    stack->addr[stack->offset++] = addr;
    return;
  }
  fprintf(stderr, "Error: Stack overflow\n");
}

size_t pop(BFStackContext *stack) {
  if (!stack) {
    fprintf(stderr, "Error: Stack is NULL\n");
    return 0;
  }
  if (stack->offset > 0) {
    return stack->addr[--stack->offset];
  }
  fprintf(stderr, "Error: Stack underflow\n");
  return 0;
}

size_t peek(BFStackContext *stack) {
  if (!stack) {
    fprintf(stderr, "Error: Stack is NULL\n");
    return 0;
  }
  if (stack->offset > 0) {
    return stack->addr[stack->offset - 1];
  }
  fprintf(stderr, "Error: Stack underflow\n");
  return 0;
}